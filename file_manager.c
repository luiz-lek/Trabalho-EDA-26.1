//
// Created by luizao on 29/05/2026.
//

#include "file_manager.h"

#include <stdlib.h>
#include <string.h>

TreeNode* tree_node_load(FILE* fp, uint32_t offset);
void tree_node_write(FILE* fp, TreeNode* node, uint32_t offset);

Person* person_data_load(FILE* fp, uint32_t offset) {
    fseek(fp, offset, SEEK_CUR);

    Person* p = person_allocate();
    fread(p->name, sizeof(char), NAME_LENGTH, fp);
    fread(&p->year, sizeof(uint16_t), 1, fp);
    return p;
}

void person_data_write(FILE *fp, uint32_t offset, Person *p) {
    fseek(fp, offset, SEEK_SET);
    fwrite(p->name, sizeof(char), NAME_LENGTH, fp);
    fwrite(&p->year, sizeof(uint16_t), 1, fp);
}

uint32_t file_size(FILE* fp) {
    uint32_t size = 0;
    const uint32_t cur_position = ftell(fp);

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);

    fseek(fp, cur_position, SEEK_SET);
    return size;
}