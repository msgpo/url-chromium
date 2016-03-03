// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/bind.h"
#include "base/message_loop/message_loop.h"
#include "base/test/launcher/unit_test_launcher.h"
#include "base/test/test_io_thread.h"
#include "base/test/test_suite.h"
#include "build/build_config.h"
#include "mojo/edk/embedder/embedder.h"
#include "mojo/edk/test/scoped_ipc_support.h"

#if defined(OS_ANDROID)
#include "base/android/jni_android.h"
#include "base/test/test_file_util.h"
#endif

int main(int argc, char** argv) {
#if defined(OS_ANDROID)
  JNIEnv* env = base::android::AttachCurrentThread();
  base::RegisterContentUriTestUtils(env);
#endif
  base::TestSuite test_suite(argc, argv);

  mojo::edk::Init();
  base::TestIOThread test_io_thread(base::TestIOThread::kAutoStart);
  scoped_ptr<mojo::edk::test::ScopedIPCSupport> ipc_support;
  ipc_support.reset(
      new mojo::edk::test::ScopedIPCSupport(test_io_thread.task_runner()));

  return base::LaunchUnitTests(
      argc, argv,
      base::Bind(&base::TestSuite::Run, base::Unretained(&test_suite)));
}
