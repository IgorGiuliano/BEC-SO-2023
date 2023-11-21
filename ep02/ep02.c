#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t boot_flag;
    uint8_t chs_start[3];
    uint8_t partition_type;
    uint8_t chs_end[3];
    uint32_t lba_start;
    uint32_t sector_count;
} PartitionEntry;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <MBR file path>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (!file) {
        perror("Unable to open file");
        return 1;
    }

    uint8_t buffer[512];
    fread(buffer, 1, 512, file);
    fclose(file);

    if (buffer[510] != 0x55 || buffer[511] != 0xAA) {
        printf("This is not a valid MBR.\n");
        return 1;
    }

    PartitionEntry* partitions = (PartitionEntry*)&buffer[446];
    for (int i = 0; i < 4; i++) {
        printf("Partition %d:\n", i+1);
        printf(" Boot Flag: 0x%02X\n", partitions[i].boot_flag);
        printf(" Start LBA: %u\n", partitions[i].lba_start);
        printf(" Sector Count: %u\n", partitions[i].sector_count);
        printf(" Partition Type: 0x%02X\n", partitions[i].partition_type);
        printf("-------------------------------\n");
    }

    return 0;
}

