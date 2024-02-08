#include <cstdio>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cinttypes>
#include <vector>


std::vector<uint32_t> make_ref_string(FILE *file, size_t &log_size) {
    uint32_t prev_page = UINT32_MAX;
    uint32_t page;
    std::vector<uint32_t> ref_string;
    
    while (fscanf(file, "%" SCNx32, &page) == 1) {
        log_size++;
        page >>= 12;
        if (page == prev_page) {
            continue;
        }
        ref_string.push_back(page);
        prev_page = page;
    }
    return ref_string;
}


int main(int argc, char **argv) {
    if (argc <= 1) {
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        return 1;
    }

    size_t log_size = 0;
    auto ref_string = make_ref_string(file, log_size);
    double ratio = double(ref_string.size()) / log_size;
    
    printf("Log tem %zu entradas, Reference String tem %zu entradas (%lfx tamanho do log)\n", log_size, ref_string.size(), ratio);

    // obter o número de falhas de página considerando diferentes tamanhos de frames livres (4, 8, 16, 32) nos algoritmos OPT e LRU
}

