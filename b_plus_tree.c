//
// Created by luizao on 29/05/2026.
//

#include "b_plus_tree.h"
#include "file_manager.h"

#include <stdlib.h>

void tree_initialize(uint8_t t, char *index, char *data, char *metadata) {
    FILE *file = fopen(index, "wb");
    if (!file) {
        perror("(fopen) falha ao abrir arquivo de índices");
        exit(-1);
    }
    fclose(file);

    file = fopen(data, "wb");
    if (!file) {
        perror("(fopen) falha ao abrir arquivo de dados.");
        exit(-1);
    }
    fclose(file);
    file = fopen(metadata, "wb");
    if (!file) {
        perror("(fopen) falha ao abrir arquivo de dados.");
        exit(-1);
    }
    uint32_t root = DISK_NULL;
    fwrite(&t, sizeof(uint8_t), 1, file);
    fwrite(&root, sizeof(uint32_t), 1, file);
    fclose(file);
}

void node_initialize(TreeNode *node, uint8_t t) {
    if (!node) return;

    node->key = malloc(sizeof(uint32_t) * (2 * t - 1));
    if (!node->key) {
        perror("malloc");
        exit(-1);
    }
    node->offset = malloc(sizeof(uint32_t) * (2 * t));
    if (!node->offset) {
        perror("malloc");
        exit(-1);
    }

    node->num_keys = 0;
    node->is_leaf = 1;
    node->next = DISK_NULL;
    for(int i = 0; i < 2 * t; i++) node->offset[i] = DISK_NULL;
}

void node_free_arrays(TreeNode *node) {
    if(node) {
        free(node->key);
        free(node->offset);
        node->key = NULL;
        node->offset = NULL;
    }
}

void node_print(TreeNode *node) {
    printf("\n\nÉ folha: %d\n", node->is_leaf);
    printf("Num chaves: %d\n", node->num_keys);
    printf("Chaves: ");
    for(int i = 0; i < node->num_keys; i++) printf("%d ", node->key[i]);
    printf("\n");
    printf("Offsets: ");
    for(int i = 0; i <= node->num_keys; i++) printf("%d ", node->offset[i]);
    printf("\n");
    printf("Próximo: %d\n", node->next);
}

uint8_t get_t(char *metadata) {
    FILE *fp = fopen(metadata, "rb+");
    if(!fp) {
        perror("(fopen) falha ao abrir metadados");
        exit(-1);
    }
    fseek(fp, 0, SEEK_SET);
    uint8_t t;
    fread(&t, sizeof(uint8_t), 1, fp);
    fclose(fp);
    return t;
}

void update_root(char *metadata, uint32_t offset_root) {
    FILE *fp = fopen(metadata, "rb+");
    if(!fp) {
        perror("(fopen) falha ao abrir metadados");
        exit(-1);
    }
    fseek(fp, sizeof(uint8_t), SEEK_SET);
    fwrite(&offset_root, sizeof(uint32_t), 1, fp);
    fclose(fp);
}

uint32_t get_root(char *metadata) {
    FILE *fp = fopen(metadata, "rb+");
    if(!fp) {
        perror("(fopen) falha ao abrir metadados");
        exit(-1);
    }

    uint32_t offset_root = DISK_NULL;
    fseek(fp, sizeof(uint8_t), SEEK_SET);
    fread(&offset_root, sizeof(uint32_t), 1, fp);
    fclose(fp);
    return offset_root;
}

// Inicializa os vetores e carrega do disco
// Obs. Liberar os vetores assim q terminar de usar a struct carregada
void node_load_alocate_arrays(TreeNode *node, uint8_t t, FILE *fp, uint32_t node_offset) {
    node_initialize(node, t);
    tree_node_load(node, t, fp, node_offset);
}

void node_save_free_arrays(TreeNode *node, uint8_t t, FILE *fp, uint32_t node_offset) {
    tree_node_save(node, t, fp, node_offset);
    node_free_arrays(node);
}

// uint32_t tree_search(FILE *index, uint8_t t, uint32_t offset, uint32_t key) {
//     if(offset == DISK_NULL) return DISK_NULL;
//
//     TreeNode node;
//     node_load_alocate_arrays(&node, t, index, offset);
//
//     int i = 0;
//     while ((i < node.num_keys) && (key > node.key[i])) i++;
//
//     const uint32_t k = node.offset[i];
//     uint32_t ret;
//
//     if ((i < node.num_keys) && (node.is_leaf) && (key == node.key[i])) ret = node.offset[i];
//     else if (node.is_leaf) ret = DISK_NULL;
//     else {
//         if (node.key[i] == key) i++;
//         ret = tree_search(index, t, node.offset[i], key);
//     }
//
//     node_free_arrays(&node);
//     return ret;
// }

uint32_t tree_search(FILE *index, uint8_t t, uint32_t offset, uint32_t key) {
    if (offset == DISK_NULL) return DISK_NULL;

    TreeNode node;
    node_load_alocate_arrays(&node, t, index, offset);

    int i = 0;
    // Encontra a posição correta (busca linear)
    while ((i < node.num_keys) && (key > node.key[i])) {
        i++;
    }

    // CASO 1: É um Nó Folha (Retorna o offset do dado ou DISK_NULL)
    if (node.is_leaf) {
        uint32_t data_offset = DISK_NULL;

        if ((i < node.num_keys) && (node.key[i] == key)) {
            data_offset = node.offset[i];
        }

        node_free_arrays(&node); // Libera a memória antes de retornar
        return data_offset;
    }

    // CASO 2: É um Nó Interno (Desce para o próximo nível)
    // Se a chave procurada for exatamente igual a uma chave de roteamento,
    // vamos para o filho à direita (offset[i + 1])
    if ((i < node.num_keys) && (node.key[i] == key)) {
        i++;
    }

    uint32_t next_child = node.offset[i]; // Captura o filho CORRETO

    node_free_arrays(&node); // Libera a memória antes de fazer a chamada recursiva

    return tree_search(index, t, next_child, key);
}

void division(TreeNode *x, TreeNode *y, TreeNode *z, uint32_t offset_z, int i, uint8_t t) {
    z->is_leaf = y->is_leaf;
    int j;

    if(!y->is_leaf) {
        z->num_keys = t-1;
        for(j=0; j<t-1; j++) {
            z->key[j] = y->key[j+t];
        }
        for(j=0; j<t; j++) {
            z->offset[j] = y->offset[j+t];
            y->offset[j+t] = DISK_NULL;
        }
    } else {
        z->num_keys = t; // z possuir uma chave a mais que y se for folha
        // Move as chaves e os offsets que apontam para o dados (pois y não é folha)
        for(j = 0; j < t; j++) {
            z->key[j] = y->key[j+t-1];
            z->offset[j] = y->offset[j+t-1];
        }
        //Caso em que y é folha, temos q passar a info para o nó da direita
        z->next = y->next; //ultima revisao: 04/2020
        y->next = offset_z;
    }

    y->num_keys = t-1;
    for(j = x->num_keys; j >= i; j--) x->offset[j+1] = x->offset[j];
    x->offset[i] = offset_z;
    for(j = x->num_keys; j >= i; j--) x->key[j] = x->key[j-1];
    x->key[i-1] = y->key[t-1];
    x->num_keys++;

    printf("\n\nNó x");
    node_print(x);
    printf("\n\nNó y");
    node_print(y);
    printf("\n\nNó z");
    node_print(z);
}

void insert_not_complete(FILE *fp, uint32_t node_offset, uint32_t id, uint32_t offset_data, uint8_t t){
    TreeNode current_node;
    node_load_alocate_arrays(&current_node, t, fp, node_offset);

    int i = current_node.num_keys - 1;
    if(current_node.is_leaf) {
        while(i >= 0 && id < current_node.key[i]) {
            current_node.key[i+1] = current_node.key[i];
            current_node.offset[i+1] = current_node.offset[i];
            i--;
        }
        current_node.key[i+1] = id;
        current_node.offset[i+1] = offset_data;
        current_node.num_keys++;
        node_save_free_arrays(&current_node, t, fp, node_offset);
        return;
    }

    while(i >= 0 && id < current_node.key[i]) i--;
    i++;

    TreeNode next_node;
    node_load_alocate_arrays(&next_node, t, fp, current_node.offset[i]);
    uint32_t next_node_offset = current_node.offset[i];

    uint32_t max_keys = 2 * t -1;
    if(next_node.num_keys == max_keys) {
        TreeNode new_node;
        node_initialize(&new_node, t);
        uint32_t new_node_offset = file_size(fp);

        division(&current_node, &next_node, &new_node, new_node_offset, i + 1, t);

        uint32_t next = next_node_offset;
        if(id > current_node.key[i]) next_node_offset = current_node.offset[i+1];

        node_save_free_arrays(&current_node, t, fp, node_offset);
        node_save_free_arrays(&next_node, t, fp, next);
        node_save_free_arrays(&new_node, t, fp, new_node_offset);
    }

    insert_not_complete(fp, next_node_offset, id, offset_data, t);
}

void tree_insert(char *index, char *metadata, uint32_t key, uint32_t offset_data) {
    FILE *fi = fopen(index,"rb+");
    if(!fi) {
        perror(index);
        exit(-1);
    }

    uint32_t offset_root = get_root(metadata);
    uint8_t t = get_t(metadata);
    if(tree_search(fi, t, offset_root, key) != DISK_NULL) {
        fclose(fi);
        return;
    }

    TreeNode root;
    node_initialize(&root, t);
    if(offset_root == DISK_NULL) {
        root.key[0] = key;
        root.offset[0] = offset_data;
        root.num_keys = 1;
        offset_root = file_size(fi);
        // node_print(&root);
        node_save_free_arrays(&root, t, fi, offset_root);
        update_root(metadata, offset_root);
        return;
    }

    tree_node_load(&root, t, fi, offset_root);
    uint32_t max_keys = 2 * t -1;
    // node_print(&root);

    if(root.num_keys == max_keys) {
        TreeNode new_root;
        node_initialize(&new_root, t);
        new_root.is_leaf = 0;
        new_root.offset[0] = offset_root;

        TreeNode new_node;
        node_initialize(&new_node, t);
        uint32_t new_node_offset = file_size(fi);

        division(&new_root, &root, &new_node, new_node_offset, 1, t);

        // node_print(&root);
        // node_print(&new_root);
        // node_print(&new_node);

        node_save_free_arrays(&new_node, t, fi, new_node_offset);
        node_save_free_arrays(&root, t, fi, offset_root);
        offset_root = file_size(fi);
        node_save_free_arrays(&new_root, t, fi, offset_root);
        update_root(metadata, offset_root);

        insert_not_complete(fi, offset_root, key, offset_data, t);
    } else {
        node_free_arrays(&root);
        insert_not_complete(fi, offset_root, key, offset_data, t);
    }
    fclose(fi);
}

void imp(FILE *fp, uint32_t node_offset, uint8_t t, int andar) {
    TreeNode current_node;
    node_load_alocate_arrays(&current_node, t, fp, node_offset);

    int i, j;
    if(!current_node.is_leaf) {
        imp(fp, current_node.offset[current_node.num_keys], t, andar+1);
        for(i = current_node.num_keys-1; i >= 0; i--) {
            for(j=0; j<=andar; j++) printf("\t");
            printf("%d\n", current_node.key[i]);
            imp(fp, current_node.offset[i], t, andar+1);
        }
    } else {
        for(i = current_node.num_keys-1; i >= 0; i--) {
            for(j = 0; j <= andar; j++) printf("\t");
            printf("%d\n", current_node.key[i]);
        }
    }

    node_free_arrays(&current_node);
}

void tree_print(char *index, char *metadata) {
    FILE *fi = fopen(index,"rb+");
    if(!fi) {
        perror(index);
        exit(-1);
    }

    uint32_t offset_root = get_root(metadata);
    uint8_t t = get_t(metadata);

    imp(fi, offset_root, t, 0);

    fclose(fi);
}