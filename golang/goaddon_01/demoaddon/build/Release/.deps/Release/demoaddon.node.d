cmd_Release/demoaddon.node := c++ -bundle -undefined dynamic_lookup -Wl,-search_paths_first -mmacosx-version-min=10.15 -arch arm64 -L./Release -stdlib=libc++  -o Release/demoaddon.node Release/obj.target/demoaddon/demoaddon.o ../demoaddon.a
