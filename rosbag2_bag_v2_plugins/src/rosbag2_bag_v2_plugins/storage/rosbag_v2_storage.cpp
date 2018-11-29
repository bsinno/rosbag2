// Copyright 2018, Bosch Software Innovations GmbH.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rosbag2_bag_v2_plugins/storage/rosbag_v2_storage.hpp"

#include <memory>
#include <string>
#include <vector>

#include "rosbag2_bag_v2_plugins/storage/convert_rosbag_message.hpp"
#include "rosbag2_storage/filesystem_helper.hpp"


namespace rosbag2_bag_v2_plugins
{

RosbagV2Storage::RosbagV2Storage()
: ros_v2_bag_(std::make_unique<rosbag::Bag>()), bag_view_(nullptr) {}

RosbagV2Storage::~RosbagV2Storage()
{
  ros_v2_bag_->close();
}

void RosbagV2Storage::open(
  const std::string & uri, rosbag2_storage::storage_interfaces::IOFlag flag)
{
  if (flag == rosbag2_storage::storage_interfaces::IOFlag::READ_WRITE) {
    throw std::runtime_error("The rosbag_v2 storage plugin can only be used to read");
  }

  ros_v2_bag_->open(uri);
  bag_view_ = std::make_unique<rosbag::View>(*ros_v2_bag_);
  bag_iterator_ = bag_view_->begin();
}

bool RosbagV2Storage::has_next()
{
  return bag_iterator_ != bag_view_->end();
}

std::shared_ptr<rosbag2_storage::SerializedBagMessage> RosbagV2Storage::read_next()
{
  auto message = convert_1_to_2(*bag_iterator_);
  bag_iterator_++;

  return message;
}

std::vector<rosbag2_storage::TopicMetadata> RosbagV2Storage::get_all_topics_and_types()
{
  std::vector<rosbag2_storage::TopicMetadata> topics_with_type;
  auto connection_info = bag_view_->getConnections();

  for (const auto & connection : connection_info) {
    rosbag2_storage::TopicMetadata topic_metadata;
    topic_metadata.name = connection->topic;
    topic_metadata.type = connection->datatype;
    // TODO(Martin-Idel-SI) add correct format when splitting up plugin
    topic_metadata.serialization_format = "";

    topics_with_type.push_back(topic_metadata);
  }

  return topics_with_type;
}

rosbag2_storage::BagMetadata RosbagV2Storage::get_metadata()
{
  std::string bag_name = ros_v2_bag_->getFileName();
  rosbag2_storage::BagMetadata metadata;
  metadata.storage_identifier = "rosbag_v2";
  metadata.bag_size = rosbag2_storage::FilesystemHelper::get_file_size(bag_name);
  metadata.relative_file_paths = {bag_name};
  metadata.duration = std::chrono::nanoseconds(
    bag_view_->getEndTime().toNSec() - bag_view_->getBeginTime().toNSec());
  metadata.starting_time = std::chrono::time_point<std::chrono::high_resolution_clock>(
    std::chrono::nanoseconds(bag_view_->getBeginTime().toNSec()));
  metadata.message_count = bag_view_->size();
  metadata.topics_with_message_count = get_topic_information();

  return metadata;
}

std::vector<rosbag2_storage::TopicInformation> RosbagV2Storage::get_topic_information()
{
  std::vector<rosbag2_storage::TopicInformation> topic_information;
  auto topics_with_type = get_all_topics_and_types();

  for (const auto & topic : topics_with_type) {
    rosbag2_storage::TopicInformation topic_info;
    rosbag::View view_with_topic_query(*ros_v2_bag_, rosbag::TopicQuery({topic.name}));
    topic_info.topic_metadata = topic;
    topic_info.message_count = view_with_topic_query.size();

    topic_information.push_back(topic_info);
  }

  return topic_information;
}

}  // namespace rosbag2_bag_v2_plugins

#include "pluginlib/class_list_macros.hpp"  // NOLINT
PLUGINLIB_EXPORT_CLASS(
  rosbag2_bag_v2_plugins::RosbagV2Storage, rosbag2_storage::storage_interfaces::ReadOnlyInterface)