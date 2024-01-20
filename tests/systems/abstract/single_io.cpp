/*
 * single_io.cpp
 *
 *  Created on: Sep 27, 2009
 *      Author: dc
 */

#include "../exposed_io_system.h"
#include <gtest/gtest.h>

namespace {
using namespace barrett;

TEST(SingleIOTest, DefaultCtor) {
  ExposedIOSystem<double> eios;
  checkDisconnected(eios);
}

} // namespace
