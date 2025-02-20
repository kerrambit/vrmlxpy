#pragma once

namespace vrml_proc::parser {

    struct BufferView {
        const char* begin;
        const char* end;

        BufferView(const char* start, const char* finish)
            : begin(start), end(finish) {}
    };
}
