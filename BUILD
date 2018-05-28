package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "spiro_test",
    srcs = [
        "spiro_test.cc",
    ],
    copts = [
      '-Wno-unused-but-set-variable',
      '-Wno-sequence-point',
      '-fpermissive',
      '-DDO_CALL_TEST1'
    ],
    deps = [
        "//libspiro:spiro",
    ],
)

cc_binary(
    name = "spiro_example",
    srcs = [
        "spiro_example.cc",
    ],
    deps = [
      "//libspiro:spiro",
    ],
)

cc_binary(
    name = "spiro_wrapper_example",
    srcs = [
        "spiro_wrapper_example.cc",
    ],
    deps = [
        ":spiro_wrapper",
    ],
)

cc_library(
  name = "spiro_wrapper",
  hdrs = [
    "spiro_wrapper.h",
  ],
  srcs = [
    "spiro_wrapper.cc",
  ],
  deps = [
    "//libspiro:spiro",
  ],
)
