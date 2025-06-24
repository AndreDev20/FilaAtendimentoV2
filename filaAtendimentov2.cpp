#include <iostream>
#include <string>
using namespace std;

#define MAX 5

struct Fila {
    float nos[MAX];
    int ini;
    int fim;
};

Fila* initFila() {
    Fila* f = new Fila;
    f->ini = 0;
    f->fim = 0;
    return f;
}

int isEmpty(Fila* f) {
    return (f->ini == f->fim);
}

int incrementa(int i) {
    return (i == MAX ? 0 : ++i);
}

int count(Fila* f) {
    int qtde = 0;
    int i = f->ini;
    while (i != f->fim) {
        qtde++;
        i = incrementa(i);
    }
    return qtde;
}

int enqueue(Fila* f, float v) {
    int podeEnfileirar = (incrementa(f->fim) != f->ini);
    if (podeEnfileirar) {
        f->nos[f->fim] = v;
        f->fim = incrementa(f->fim);
    }
    return podeEnfileirar;
}

int dequeue(Fila* f) {
    int ret;
    if (isEmpty(f)) {
        ret = -1;
    } else {
        ret = f->nos[f->ini];
        f->ini = incrementa(f->ini);
    }
    return ret;
}

void printFila(Fila* f) {
    int i = f->ini;
    while (i != f->fim) {
        cout << f->nos[i] << " ";
        i = incrementa(i);
    }
    cout << endl;
}

void freeFila(Fila* f) {
    delete f;
}

struct Guiche {
    int id;
    Fila* senhasAtendidas;
    Guiche* prox;
};

Guiche* criarGuiche(int id) {
    Guiche* g = new Guiche;
    g->id = id;
    g->senhasAtendidas = initFila();
    g->prox = nullptr;
    return g;
}

void adicionarGuiche(Guiche*& lista, int id) {
    Guiche* novo = criarGuiche(id);
    novo->prox = lista;
    lista = novo;
}

Guiche* buscarGuiche(Guiche* lista, int id) {
    while (lista != nullptr) {
        if (lista->id == id) return lista;
        lista = lista->prox;
    }
    return nullptr;
}

int contarGuiches(Guiche* lista) {
    int count = 0;
    while (lista != nullptr) {
        count++;
        lista = lista->prox;
    }
    return count;
}

void listarSenhasAtendidas(Guiche* guiche) {
    if (guiche == nullptr) {
        cout << "Guichê não encontrado." << endl;
    } else {
        cout << "Senhas atendidas pelo Guichê " << guiche->id << ": ";
        printFila(guiche->senhasAtendidas);
    }
}

int main() {
    int senha = 0;
    Fila* senhasGeradas = initFila();
    Guiche* guiches = nullptr;
    bool isTerminated = false;

    while (!isTerminated) {
        int op = -1;

        cout << "\n===== SISTEMA DE ATENDIMENTO 2.0 =====\n";
        cout << "[0] Sair\n[1] Gerar senha\n[2] Abrir guichê\n[3] Realizar atendimento\n[4] Listar senhas atendidas\n";
        cout << "\nSenhas pendentes: " << count(senhasGeradas);
        cout << " | Guichês abertos: " << contarGuiches(guiches) << endl;

        cout << "Escolha uma opcao: ";
        cin >> op;

        switch (op) {
        case 0:
            if (count(senhasGeradas) == 0) {
                cout << "\nEncerrando o sistema.\n";
                int total = 0;
                for (Guiche* g = guiches; g != nullptr; g = g->prox) {
                    total += count(g->senhasAtendidas);
                }
                cout << "Total de senhas atendidas: " << total << endl;
                isTerminated = true;
            } else {
                cout << "Ainda há senhas pendentes. Atenda todas antes de sair.\n";
            }
            break;

        case 1:
            if (enqueue(senhasGeradas, ++senha)) {
                cout << "Senha " << senha << " gerada com sucesso.\n";
            } else {
                cout << "Fila cheia. Realize atendimentos primeiro.\n";
                senha--;
            }
            break;

        case 2: {
            int id;
            cout << "Digite o ID do novo guichê: ";
            cin >> id;
            if (buscarGuiche(guiches, id)) {
                cout << "Guichê já existe.\n";
            } else {
                adicionarGuiche(guiches, id);
                cout << "Guichê " << id << " aberto com sucesso.\n";
            }
            break;
        }

        case 3: {
            if (isEmpty(senhasGeradas)) {
                cout << "Fila vazia. Gere uma senha antes.\n";
                break;
            }

            int id;
            cout << "Digite o ID do guichê que irá atender: ";
            cin >> id;

            Guiche* g = buscarGuiche(guiches, id);
            if (!g) {
                cout << "Guichê não encontrado.\n";
                break;
            }

            int senhaAtendida = dequeue(senhasGeradas);
            enqueue(g->senhasAtendidas, senhaAtendida);

            cout << "Guichê " << id << " atendeu a senha: " << senhaAtendida << endl;
            break;
        }

        case 4: {
            int id;
            cout << "Digite o ID do guichê: ";
            cin >> id;
            Guiche* g = buscarGuiche(guiches, id);
            listarSenhasAtendidas(g);
            break;
        }

        default:
            cout << "Opção inválida. Tente novamente.\n";
            break;
        }
    }

    freeFila(senhasGeradas);
    while (guiches != nullptr) {
        Guiche* temp = guiches;
        guiches = guiches->prox;
        freeFila(temp->senhasAtendidas);
        delete temp;
    }

    return 0;
}
