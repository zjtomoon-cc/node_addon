{
    "targets": [
        {
            "target_name": "demoaddon",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "sources": [ "demoaddon.cc" ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")"
            ],
            'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
            "libraries": [
                "../demoaddon.a"
            ],
        }
    ]
}