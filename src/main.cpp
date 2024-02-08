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

size_t lru_faults(std::vector<u32> &ref_string, size_t max_frames) {
    size_t faults = 0;
    std::vector<u32> frames;

    for (u32 page : ref_string) {
        faults++;
        for (auto frame = frames.begin(); frame < frames.end(); frame++) {
            // ja esta carregada, retira do meio da pilha
            if (page == *frame) {
                faults--;
                frames.erase(frame);
                break;
            }
        }
        // se está cheio, remove a base da pilha
        if (frames.size() == max_frames) {
            frames.erase(frames.begin());
        }
        // sempre vai para o topo da pilha
        frames.push_back(page);
    }
    return faults;
}


size_t opt_faults(std::vector<u32> &ref_string, size_t max_frames) {
    size_t faults = 0;
    // TODO
    return faults;
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
    double ratio = double(ref_string.size()) / log_size;
    
    printf("%zu entradas no Log\n", log_size);
    printf("%zu entradas na Reference String (%lfx tamanho do log)\n", ref_string.size(), ratio);

    // obter o número de falhas de página considerando diferentes tamanhos de frames livres (4, 8, 16, 32) nos algoritmos OPT e LRU
    
    for (size_t max_frames = 4; max_frames <= 32; max_frames *= 2) {
        size_t num_faults = lru_faults(ref_string, max_frames);
        double fail_ratio = double(num_faults) / ref_string.size();
        printf("Com %2zu frames livres, LRU sofreu %zu falhas (taxa de falha %lf)\n", max_frames, num_faults, fail_ratio);
    }
    for (size_t max_frames = 4; max_frames <= 32; max_frames *= 2) {
        size_t num_faults = opt_faults(ref_string, max_frames);
        double fail_ratio = double(num_faults) / ref_string.size();
        printf("Com %2zu frames livres, OPT sofreu %zu falhas (taxa de falha %lf)\n", max_frames, num_faults, fail_ratio);
    }
}

