//
// Created by luizao on 29/05/2026.
//

#include "../include/b_plus_tree.h"
#include "../include/file_manager.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Para a descidas reursivas, são carregados apenas os nós atual.
// Assim que o nó não for mais necessário, ele é liberao da memória.
// Lê os campos na ordem q estão declarados na struct do nó.

bool tree_node_read(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset) {
    if(offset == DISK_NULL) return false;

    int keys_length = 2 * t - 1, child_length = 2 * t;

    // Cálculo da quantidades de bytes que o nó ocupa
    size_t buffer_size = sizeof(uint8_t) + sizeof(uint16_t)
                         + (keys_length * sizeof(uint32_t))
                         + (child_length * sizeof(uint32_t))
                         + sizeof(uint32_t);

    uint8_t *buffer = malloc(buffer_size); // uint8_t porque cada índice do vetor aramazena 1 byte
    if(!buffer) {
        perror("(malloc) falha ao alocar buffer para leitura de nó.");
        exit(-1);
    }

    bool read_success = read_data(fp, offset, buffer, buffer_size); // Lê todos o bytes da struct gravada no disco

    if(!read_success) {
        free(buffer);
        return false;
    }

    uint32_t pos = 0;

    // Copia os bytes de is_leaf do buffer
    memcpy(&node->is_leaf, buffer + pos, sizeof(uint8_t));
    pos += sizeof(uint8_t);

    // Copia os bytes de num_keys do buffer
    memcpy(&node->num_keys, buffer + pos, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    // Copia os bytes do vetor de chaves
    memcpy(node->key, buffer + pos, sizeof(uint32_t) * keys_length);
    pos += sizeof(uint32_t) * keys_length;

    // Copia o vetor de offsets
    memcpy(node->offset, buffer + pos, sizeof(uint32_t) * child_length);
    pos += sizeof(uint32_t) * child_length;

    // Copia o offset do proximo
    memcpy(&node->next, buffer + pos, sizeof(uint32_t));

    free(buffer);
    return true;
}

// Mesma lógica de cima, mas pra escrita
void tree_node_write(TreeNode *node, uint8_t t, FILE *fp, uint32_t offset) {
    int keys_length = 2 * t - 1, child_length = 2 * t;
    size_t buffer_size = sizeof(uint8_t) + sizeof(uint16_t)
                         + (keys_length * sizeof(uint32_t))
                         + (child_length * sizeof(uint32_t))
                         + sizeof(uint32_t);

    uint8_t *buffer = malloc(buffer_size);
    if(!buffer) {
        perror("(malloc) falha ao alocar buffer para escrita de nó.");
        exit(-1);
    }

    uint32_t pos = 0; // Marca o offset pro inicio do novo bloco de escrita

    // Escreve no buffer os bytes de is_leaf
    memcpy(buffer + pos, &node->is_leaf, sizeof(uint8_t));
    pos += sizeof(uint8_t);

    // Escreve no buffer os bytes de num_chaves
    memcpy(buffer + pos, &node->num_keys, sizeof(uint16_t));
    pos += sizeof(uint16_t);

    // Escreve no buffer os bytes do vetor de chaves
    memcpy(buffer + pos, node->key,  sizeof(uint32_t) * keys_length);
    pos += sizeof(uint32_t) * keys_length;

    // Escreve no buffer os bytes vetor de offsets
    memcpy(buffer + pos, node->offset,  sizeof(uint32_t) * child_length);
    pos += sizeof(uint32_t) * child_length;

    // Escreve no buffer os bytes do offset do proximo
    memcpy(buffer + pos, &node->next, sizeof(uint32_t));

    // Grava o buffer com todos os bytes de node
    write_data(fp, offset, buffer, buffer_size);
    free(buffer);
}

void load_header(const char *metadata_file, Header *h) {
    FILE *fp = open_file(metadata_file, "rb");
    read_data(fp, 0, h, sizeof(Header));
    fclose(fp);
}

void save_header(const char *metadata_file, Header *h) {
    FILE *fp = open_file(metadata_file, "wb");
    write_data(fp, 0, h, sizeof(Header));
    fclose(fp);
}

void tree_initialize(uint8_t t, char *index, char *data, char *metadata) {
    FILE *file = open_file(index, "wb");
    fclose(file);
    file = open_file(data, "wb");
    fclose(file);

    Header h;
    h.root = DISK_NULL;
    h.t = t;
    h.first_leaf = DISK_NULL;
    h.last_id_generated = 0;
    save_header(metadata, &h);
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

void update_root(char *file_metadata, uint32_t new_root) {
    Header h;
    load_header(file_metadata, &h);
    h.root = new_root;
    save_header(file_metadata, &h);
}

void update_first_leaf(char *file_metadata, uint32_t new_first_leaf) {
    Header h;
    load_header(file_metadata, &h);
    h.first_leaf = new_first_leaf;
    save_header(file_metadata, &h);
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

// Inicializa os vetores e carrega do disco
// Obs. Liberar os vetores assim q terminar de usar a struct carregada
void node_initialize_and_load(TreeNode *node, uint8_t t, FILE *fp, uint32_t node_offset) {
    node_initialize(node, t);
    tree_node_read(node, t, fp, node_offset);
}

void node_free_and_save(TreeNode *node, uint8_t t, FILE *fp, uint32_t node_offset) {
    tree_node_write(node, t, fp, node_offset);
    node_free_arrays(node);
}

uint32_t search_key(FILE *index, uint8_t t, uint32_t offset, uint32_t key) {
    if (offset == DISK_NULL) return DISK_NULL;

    TreeNode node;
    node_initialize_and_load(&node, t, index, offset);

    int i = 0;
    // Encontra a posição correta (busca linear)
    while ((i < node.num_keys) && (key > node.key[i])) {
        i++;
    }

    // CASO 1: É um Nó Folha (Retorna o offset do dado ou DISK_NULL)
    if (node.is_leaf) {
        uint32_t data_offset = DISK_NULL;

        if ((i < node.num_keys) && (node.key[i] == key)) data_offset = node.offset[i];

        node_free_arrays(&node); // Libera a memória antes de retornar
        return data_offset;
    }

    // CASO 2: É um Nó Interno (Desce para o próximo nível)
    // Se a chave procurada for exatamente igual a uma chave de roteamento,
    // vamos para o filho à direita (offset[i + 1])
    if ((i < node.num_keys) && (node.key[i] == key)) i++;

    uint32_t next_child = node.offset[i]; // Captura o filho CORRETO

    node_free_arrays(&node); // Libera a memória antes de fazer a chamada recursiva

    return search_key(index, t, next_child, key);
}

uint32_t tree_search(char *index, char *metadata, uint32_t key) {
    FILE *fpi = open_file(index, "rb");

    Header h;
    load_header(metadata, &h);

    uint32_t offset_data = search_key(fpi, h.t, h.root, key);
    fclose(fpi);
    return offset_data;
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
}

void insert_not_complete(FILE *fp, uint32_t node_offset, uint32_t id, uint32_t offset_data, uint8_t t){
    TreeNode current_node;
    node_initialize_and_load(&current_node, t, fp, node_offset);

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
        node_free_and_save(&current_node, t, fp, node_offset);
        return;
    }

    while(i >= 0 && id < current_node.key[i]) i--;
    i++;

    TreeNode next_node;
    node_initialize_and_load(&next_node, t, fp, current_node.offset[i]);
    uint32_t next_node_offset = current_node.offset[i];

    uint32_t max_keys = 2 * t -1;
    if(next_node.num_keys == max_keys) {
        TreeNode new_node;
        node_initialize(&new_node, t);
        uint32_t new_node_offset = file_size(fp);

        division(&current_node, &next_node, &new_node, new_node_offset, i + 1, t);

        uint32_t next = next_node_offset;
        if(id > current_node.key[i]) next_node_offset = current_node.offset[i+1];

        node_free_and_save(&current_node, t, fp, node_offset);
        node_free_and_save(&next_node, t, fp, next);
        node_free_and_save(&new_node, t, fp, new_node_offset);
    } else {
        node_free_arrays(&current_node);
        node_free_arrays(&next_node);
    }

    insert_not_complete(fp, next_node_offset, id, offset_data, t);
}

void tree_insert(char *index, char *metadata, uint32_t key, uint32_t offset_data) {
    FILE *fi = open_file(index, "rb+");

    Header h;
    load_header(metadata, &h);
    const uint8_t t = h.t;
    uint32_t offset_root = h.root;

    if(search_key(fi, t, offset_root, key) != DISK_NULL) {
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

        node_free_and_save(&root, t, fi, offset_root);
        fclose(fi);

        h.root = offset_root;
        h.first_leaf = offset_root;
        save_header(metadata, &h);
        return;
    }

    tree_node_read(&root, t, fi, offset_root);
    uint32_t max_keys = 2 * t -1;

    if(root.num_keys == max_keys) {
        TreeNode new_root;
        node_initialize(&new_root, t);
        new_root.is_leaf = 0;
        new_root.offset[0] = offset_root;

        TreeNode new_node;
        node_initialize(&new_node, t);
        uint32_t new_node_offset = file_size(fi);

        division(&new_root, &root, &new_node, new_node_offset, 1, t);

        node_free_and_save(&new_node, t, fi, new_node_offset);
        node_free_and_save(&root, t, fi, offset_root);
        offset_root = file_size(fi);
        node_free_and_save(&new_root, t, fi, offset_root);
        h.root = offset_root;
        save_header(metadata, &h);

        insert_not_complete(fi, offset_root, key, offset_data, t);
    } else {
        node_free_arrays(&root);
        insert_not_complete(fi, offset_root, key, offset_data, t);
    }
    fclose(fi);
}

void imp(FILE *fp, uint32_t node_offset, uint8_t t, int andar) {
    TreeNode current_node;
    node_initialize_and_load(&current_node, t, fp, node_offset);

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
    FILE *fi = open_file(index, "rb+");

    Header h;
    load_header(metadata, &h);

    uint32_t offset_root = h.root;
    if(offset_root == DISK_NULL) {
        printf("Árvore vazia!");
    } else {
        uint8_t t = h.t;
        imp(fi, offset_root, t, 0);
    }

    fclose(fi);
}

void case_1(TreeNode *node, int i) {
    for(; i < node->num_keys-1; i++) {
        node->key[i] = node->key[i+1];
        node->offset[i] = node->offset[i+1];
    }
    node->num_keys--;
}

void case_3a_less_than_num_keys(TreeNode *current_node, TreeNode *y, TreeNode *z, uint8_t t, int i) {
    y->num_keys++;
    if(!y->is_leaf) {
        y->key[t-1] = current_node->key[i];   //dar a y a chave i da arv
        current_node->key[i] = z->key[0];     //dar a arv uma chave de z

        int j;
        for(j = 0; j < z->num_keys-1; j++)  //ajustar chaves de z
            z->key[j] = z->key[j+1];
        y->offset[y->num_keys] = z->offset[0]; //enviar ponteiro menor de z para o novo elemento em y
        for(j = 0; j < z->num_keys; j++)       //ajustar filhos de z
            z->offset[j] = z->offset[j+1];
        z->num_keys--;

        return;
    }
    // y é folha e z também (são irmãos)
    current_node->key[i] = z->key[0] + 1;
    y->key[t-1] = z->key[0];
    y->offset[t-1] = z->offset[0];

    int j;
    //ajustar chaves de z
    for(j = 0; j < z->num_keys-1; j++) {
        z->key[j] = z->key[j+1];
        z->offset[j] = z->offset[j+1];
    }
    z->num_keys--;
}

void case_3a_equal_num_keys(TreeNode *current_node, TreeNode *y, TreeNode *z, uint8_t t, int i) {
    int j;
    if(!y->is_leaf) {
        for(j = y->num_keys; j > 0; j--)               //encaixar lugar da nova chave
            y->key[j] = y->key[j-1];
        for(j = y->num_keys+1; j>0; j--) //encaixar lugar dos filhos da nova chave
            y->offset[j] = y->offset[j-1];
        y->key[0] = current_node->key[i-1];    //dar a y a chave i da arv
        current_node->key[i-1] = z->key[z->num_keys-1];   //dar a arv uma chave de z
        y->offset[0] = z->offset[z->num_keys];
    } else {
        for(j = y->num_keys; j > 0; j--) { //encaixar lugar da nova chave
            y->key[j] = y->key[j-1];
            y->offset[j] = y->offset[j-1];
        }
        current_node->key[i-1] = z->key[z->num_keys-1];
        y->key[0] = z->key[z->num_keys-1];
        y->offset[0] = z->offset[z->num_keys-1];
    }

    y->num_keys++;
    z->num_keys--;
}

bool case_3b_less_than_num_keys(TreeNode *current_node, TreeNode *y, TreeNode *z, uint8_t t, int i) {
    if(!y->is_leaf) {
        y->key[t-1] = current_node->key[i]; //pegar chave [i] e coloca ao final de filho[i]
        y->num_keys++;
    }
    int j = 0;
    while(j < t-1) {
        if(!y->is_leaf) {
            y->key[t+j] = z->key[j];
            y->offset[t+j] = z->offset[j];
        }
        else {
            y->key[t+j-1] = z->key[j];
            y->offset[t+j-1] = z->offset[j];
        }
        y->num_keys++;
        j++;
    }
    y->next = z->next;
    if(!y->is_leaf) {
        for(j=0; j<t; j++) {
            y->offset[t+j] = z->offset[j];
            z->offset[j] = DISK_NULL; //ultima revisao: 04/2020
        }
        //TARVBM_libera(z); 07/2024
    }
    // TARVBM_libera(z); // 07/2024
    for(j = i; j < current_node->num_keys-1; j++) { //limpar refer�ncias de i
        current_node->key[j] = current_node->key[j+1];
        current_node->offset[j+1] = current_node->offset[j+2];
    }
    current_node->offset[current_node->num_keys] = DISK_NULL;
    current_node->num_keys--;

    if(current_node->num_keys == 0) return true;
    return false;
}

bool case_3b_equal_num_keys(TreeNode *current_node, TreeNode *y, TreeNode *z, uint8_t t, int i) {
    if(!y->is_leaf){
        if(i == current_node->num_keys){
            z->key[t-1] = current_node->key[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
        }else{
            z->key[t-1] = current_node->key[i];   //pegar chave [i] e poe ao final de filho[i-1]
        }
        z->num_keys++;
    }
    int j = 0;
    while(j < t-1){
        if(!y->is_leaf) z->key[t+j] = y->key[j];
        else {
            z->key[t+j-1] = y->key[j];
            z->offset[t+j-1] = y->offset[j];
        }
        z->num_keys++;
        j++;
    }
    z->next = y->next;
    if(!z->is_leaf){
        for(j=0; j<t; j++){
            z->offset[t+j] = y->offset[j];
            y->offset[j] = DISK_NULL; //ultima revisao: 04/2020
        }
    }

    current_node->offset[current_node->num_keys] = DISK_NULL;
    current_node->num_keys--;

    if(!current_node->num_keys) return true;

    return false;
}

void aux_tree_remove(FILE *fp, char *metadata, uint32_t offset_current_node, uint32_t key, uint8_t t) {
    if(offset_current_node == DISK_NULL) return;

    TreeNode current_node;
    node_initialize_and_load(&current_node, t, fp, offset_current_node);

    int i;
    for(i = 0; i < current_node.num_keys && current_node.key[i] < key; i++);

    if((i < current_node.num_keys) && (key == current_node.key[i]) && (current_node.is_leaf)) { //CASO 1
        printf("\nCASO 1\n");
        case_1(&current_node, i);

        if(current_node.num_keys > 0) node_free_and_save(&current_node, t, fp, offset_current_node);
        // Só fica com quantidade igual a 0 se a folha for a raiz
        else {
            update_root(metadata, DISK_NULL);
            update_first_leaf(metadata, DISK_NULL);
        }
        return;
    }

    if((i < current_node.num_keys) && (key == current_node.key[i])) i++;
    TreeNode y, z;
    node_initialize_and_load(&y, t, fp, current_node.offset[i]);
    uint32_t offset_y = current_node.offset[i];
    uint32_t offset_next = offset_y;

    if (y.num_keys == t - 1) { // CASOS 3A e 3B
        bool solved = false;

        // 1. Tenta o Irmão da DIREITA (Caso 3A - Empréstimo da Direita)
        if (i < current_node.num_keys) {
            node_initialize_and_load(&z, t, fp, current_node.offset[i + 1]);
            if (z.num_keys >= t) {
                printf("\nCASO 3A: Emprestimo do irmao da direita\n");
                case_3a_less_than_num_keys(&current_node, &y, &z, t, i);
                node_free_and_save(&y, t, fp, current_node.offset[i]);
                node_free_and_save(&z, t, fp, current_node.offset[i + 1]);
                node_free_and_save(&current_node, t, fp, offset_current_node);
                solved = true;
            } else {
                // Se o da direita existe mas não tem chaves para emprestar,
                // liberamos os vetores vamos guardá-lo para o caso 3nextB daqui a pouco!
                node_free_arrays(&z);
            }
        }

        // 2. Tenta o Irmão da ESQUERDA (Caso 3A - Empréstimo da Esquerda)
        if (!solved && i > 0) {
            node_initialize_and_load(&z, t, fp, current_node.offset[i - 1]);
            if (z.num_keys >= t) {
                printf("\nCASO 3A: Emprestimo do irmao da esquerda\n");
                case_3a_equal_num_keys(&current_node, &y, &z, t, i);
                node_free_and_save(&y, t, fp, current_node.offset[i]);
                node_free_and_save(&z, t, fp, current_node.offset[i - 1]);
                node_free_and_save(&current_node, t, fp, offset_current_node);
                solved = true;
            } else {
                node_free_arrays(&z);
            }
        }

        // 3. Se ninguém pôde emprestar, faz a CONCATENAÇÃO (Caso 3B)
        if (!solved) {
            if (i < current_node.num_keys) {
                // Tem irmão na direita? Fundo com ele!
                printf("\nCASO 3B: Fundindo com irmao da direita\n");
                node_initialize_and_load(&z, t, fp, current_node.offset[i + 1]);

                bool is_new_root = case_3b_less_than_num_keys(&current_node, &y, &z, t, i);

                if (is_new_root) {
                    update_root(metadata, offset_y);
                }
                else node_free_and_save(&current_node, t, fp, offset_current_node);

                node_free_and_save(&y, t, fp, offset_y);
                node_free_arrays(&z);
            } else {
                uint32_t offset_z = current_node.offset[i-1];
                printf("Caso 3B: Fundido com o irmçao da esquerda\n");
                node_initialize_and_load(&z, t, fp, current_node.offset[i-1]);

                bool is_new_root = case_3b_equal_num_keys(&current_node, &y, &z, t, i);

                if(is_new_root) update_root(metadata, offset_z);
                else node_free_and_save(&current_node, t, fp, offset_current_node);

                node_free_and_save(&z, t, fp, offset_z);
                offset_next = offset_z;
            }
        }
    } else {
        node_free_arrays(&current_node);
        node_free_arrays(&y);
    }

    aux_tree_remove(fp, metadata, offset_next, key, t);
}

void tree_remove(char *index, char *metadata, uint32_t key) {
    Header h;
    load_header(metadata, &h);

    uint32_t root = h.root;
    uint8_t t = h.t;
    FILE *fp = open_file(index, "rb+");

    aux_tree_remove(fp, metadata, root, key, t);
    fclose(fp);
}