{
  "targets": [
    {
      "target_name": "user_existence",
      "cflags": [ "-fno-exceptions" ],
      "cflags_cc": [ "-fno-exceptions" ],
      "sources": [ "user_existence.cc" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}