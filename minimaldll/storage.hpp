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

#ifndef MINIMALDLL_STORAGE_HPP_
#define MINIMALDLL_STORAGE_HPP_

#include <cstdint>

class StorageInterface
{
public:
  virtual uint64_t random_number() = 0;
};

class Storage : public StorageInterface
{
public:
  uint64_t random_number() override;
};


#endif //MINIMALDLL_STORAGE_HPP_
