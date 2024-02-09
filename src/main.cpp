#include <cstdio>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cinttypes>
#include <vector>
#include <algorithm>
typedef uint32_t u32;

std::vector<u32> make_ref_string(FILE *file, size_t &log_size) {
    u32 prev_page = UINT32_MAX;
    u32 page;
    std::vector<u32> ref_string;
    
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

size_t lru_hits(std::vector<u32> &ref_string, size_t max_frames) {
    size_t cache_hits = 0;
    std::vector<u32> frames;

    for (u32 page : ref_string) {
        // se ja esta carregada, retira do meio da pilha
        auto search = std::find(frames.begin(), frames.end(), page);
        if (search != frames.end()) {
            cache_hits++;
            frames.erase(search);
        }

        // se está cheio, remove a base da pilha
        if (frames.size() == max_frames) {
            frames.erase(frames.begin());
        }
        // sempre vai para o topo da pilha
        frames.push_back(page);
    }
    return cache_hits;
}


size_t opt_hits(std::vector<u32> &ref_string, size_t max_frames) {
    size_t cache_hits = 0;
    // TODO
    return cache_hits;
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
    std::vector<uint32_t> ref_string = make_ref_string(file, log_size);
    size_t ref_size = ref_string.size();
    double ratio = double(ref_size) / log_size;
    
    printf("%zu entradas no Log\n", log_size);
    printf("%zu entradas na Reference String (%lfx tamanho do log)\n", ref_string.size(), ratio);

    for (size_t max_frames = 4; max_frames <= 32; max_frames *= 2) {
        size_t num_hits = lru_hits(ref_string, max_frames);
        double fail_ratio = double(ref_size - num_hits) / ref_string.size();
        printf("Com %2zu frames livres, LRU sofreu %zu falhas (taxa de falha %lf)\n", max_frames, num_hits, fail_ratio);
    }

    for (size_t max_frames = 4; max_frames <= 32; max_frames *= 2) {
        size_t num_hits = opt_hits(ref_string, max_frames);
        double fail_ratio = double(ref_size - num_hits) / ref_string.size();
        printf("Com %2zu frames livres, OPT sofreu %zu falhas (taxa de falha %lf)\n", max_frames, num_hits, fail_ratio);
    }
}

