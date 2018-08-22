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

#include <iostream>
#include <memory>

#include "pluginlib/class_loader.hpp"
#include "storage.hpp"

int main()
{
  auto class_loader = std::make_shared<pluginlib::ClassLoader<StorageInterface>>(
    "test_library", "StorageInterface");

  auto test = class_loader->createSharedInstance("my_storage_plugin");
  if (!test) {
    return 1;
  }
  std::cout << "Random number: " << test->random_number() << std::endl;

  return 0;
}
