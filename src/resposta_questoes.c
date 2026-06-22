#include "../include/resposta_questoes.h"
#include "../include/data_base_manipulation.h"
#include "../include/file_manager.h"
#include  "../include/hash_manipulation.h"
#include "../include/linked_list.h"
#include "../include/person.h"

// A
static void combine_persons(LinkedList *l, Hash *h) {
    Node *current = l->head;
    Node *rest = NULL;

    TupleIDs worked_together;

    while(current) {
        rest = current->next;
        uint32_t id_current = *(uint32_t*)current->data;
        while (rest) {
            uint32_t id_rest = *(uint32_t*)rest->data;
            if (id_current == id_rest) {
                rest = rest->next;
                continue;
            }
            if (id_rest < id_current) {
                worked_together.val1 = id_rest;
                worked_together.val2 = id_current;
            } else {
                worked_together.val1 = id_current;
                worked_together.val2 = id_rest;
            }

            int *lookup = h->lookup(h, &worked_together);
            int amount = 1;

            if (lookup == NULL) {
                h->insert(h, &worked_together, &amount);
            } else {
                amount = *lookup + 1;
                h->update_valor(h, &worked_together, &amount);
            }

            rest = rest->next;
        }
        current = current->next;
    }
}

void print_worked_together(TreeContext *person_tree, Hash *h) {
    for (int i = 0; i < h->ctx.table_size; i++) {
        HashNode *current = h->ctx.table[i];
        while (current) {
            TupleIDs *cur_rel = current->key;

            uint32_t offset = tree_search(person_tree, cur_rel->val1);
            Person p;
            read_data(person_tree->fp_data, offset, &p, sizeof(Person));
            printf("[%s, ", p.name);
            offset = tree_search(person_tree, cur_rel->val2);
            read_data(person_tree->fp_data, offset, &p, sizeof(Person));
            printf("%s] Qtd: %d\n", p.name, *(int*)current->value);
            current = current->next;
        }
    }
}

static void fill_list(LinkedList *l, FILE *rel_data, uint32_t head) {
    if (head == DISK_NULL) return;

    uint32_t current = head;
    Relationship relationship;
    while (current != DISK_NULL) {
        read_data(rel_data, current, &relationship, sizeof(Relationship));
        l->add_first(l, &relationship.person_id);
        current = relationship.offset_next_person;
    }
}

static void generate_persons_combination(TreeNode *node, HashContext *movie_relations, FILE *fp_realtions, Hash *h) {
    LinkedList list;
    list_initialize(&list, UINT32);

    for(int i = 0; i < node->num_keys; i++) {
        uint32_t head_rel = hash_disk_lookup(movie_relations, &node->key[i]);
        fill_list(&list, fp_realtions, head_rel);
        combine_persons(&list, h);
        list.clear(&list);
    }
}

void pessoas_trabalharam_juntas(DB *db) {
    Hash persons_worked_together;
    hash_create(&persons_worked_together, TUPLE_INT, 101);

    TreeContext movies_tree = db->tree_movie_context;
    uint32_t offset_cur = movies_tree.header.first_leaf;

    TreeNode node_cur;
    node_initialize(&node_cur, db->tree_movie_context.header.t);

    while(offset_cur != DISK_NULL) {
        tree_node_read(&node_cur, movies_tree.header.t, movies_tree.fp_index, offset_cur);
        generate_persons_combination(&node_cur, &db->hash_movie_relations_context,
                                      db->fp_relations_data, &persons_worked_together);
        offset_cur = node_cur.next;
    }
    print_worked_together(&db->tree_person_context, &persons_worked_together);
}