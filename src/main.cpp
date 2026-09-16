#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Astronauta {
private:
    string cpf;
    string nome;
    int idade;
    bool vivo;
    bool disponivel;

public:
    Astronauta(string c, string n, int i) {
        cpf = c;
        nome = n;
        idade = i;
        vivo = true;
        disponivel = true;
    }

    string getCpf() const { return cpf; }
    string getNome() const { return nome; }
    int getIdade() const { return idade; }
    bool estaVivo() const { return vivo; }
    bool estaDisponivel() const { return disponivel; }
    void embarcar() { disponivel = false; }
    void desembarcar() {
        if (vivo) disponivel = true;
    }
    void morrer() {
        vivo = false;
        disponivel = false;
    }
};

class Voo {
private:
    int codigo;
    string estado;
    vector<string> cpfs;

public:
    Voo(int c) {
        codigo = c;
        estado = "planejado";
    }

    int getCodigo() const { return codigo; }
    string getEstado() const { return estado; }
    int getQuantidadeAstronautas() const { return cpfs.size(); }
    string getCpf(int posicao) const { return cpfs[posicao]; }
    bool temAstronauta(string cpf) const {
        for (int i = 0; i < cpfs.size(); i++) {
            if (cpfs[i] == cpf) return true;
        }
        return false;
    }
    void adicionarAstronauta(string cpf) { cpfs.push_back(cpf); }
    bool removerAstronauta(string cpf) {
        for (int i = 0; i < cpfs.size(); i++) {
            if (cpfs[i] == cpf) {
                cpfs.erase(cpfs.begin() + i);
                return true;
            }
        }
        return false;
    }
    void lancar() { estado = "em curso"; }
    void finalizar() { estado = "finalizado com sucesso"; }
    void explodir() { estado = "finalizado com explosao"; }
};

class Agencia {
private:
    vector<Astronauta> astronautas;
    vector<Voo> voos;

    int buscarAstronauta(string cpf) const {
        for (int i = 0; i < astronautas.size(); i++) {
            if (astronautas[i].getCpf() == cpf) return i;
        }
        return -1;
    }
    int buscarVoo(int codigo) const {
        for (int i = 0; i < voos.size(); i++) {
            if (voos[i].getCodigo() == codigo) return i;
        }
        return -1;
    }
    // A ordem destas verificacoes faz parte da especificacao.
    bool conferirVoo(int posicao, int codigo, string estado) const {
        if (posicao == -1) {
            cout << "ERRO: voo " << codigo << " nao cadastrado" << endl;
            return false;
        }
        if (voos[posicao].getEstado() != estado) {
            cout << "ERRO: voo " << codigo << " nao esta " << estado << endl;
            return false;
        }
        return true;
    }
    bool conferirAstronauta(int posicao, string cpf) const {
        if (posicao == -1) {
            cout << "ERRO: astronauta " << cpf << " nao cadastrado" << endl;
            return false;
        }
        return true;
    }

public:
    void cadastrarAstronauta(string cpf, string nome, int idade) {
        if (buscarAstronauta(cpf) != -1) {
            cout << "ERRO: astronauta com CPF " << cpf << " ja cadastrado" << endl;
            return;
        }
        astronautas.push_back(Astronauta(cpf, nome, idade));
        cout << "OK: astronauta " << cpf << " cadastrado" << endl;
    }
    void cadastrarVoo(int codigo) {
        if (buscarVoo(codigo) != -1) {
            cout << "ERRO: voo " << codigo << " ja cadastrado" << endl;
            return;
        }
        voos.push_back(Voo(codigo));
        cout << "OK: voo " << codigo << " cadastrado" << endl;
    }
    void adicionarAstronauta(string cpf, int codigo) {
        int a = buscarAstronauta(cpf);
        int v = buscarVoo(codigo);
        if (!conferirAstronauta(a, cpf)) return;
        if (!conferirVoo(v, codigo, "planejado")) return;
        if (!astronautas[a].estaVivo()) {
            cout << "ERRO: astronauta " << cpf << " esta morto" << endl;
            return;
        }
        if (voos[v].temAstronauta(cpf)) {
            cout << "ERRO: astronauta " << cpf << " ja esta no voo " << codigo << endl;
            return;
        }
        voos[v].adicionarAstronauta(cpf);
        cout << "OK: astronauta " << cpf << " adicionado ao voo " << codigo << endl;
    }
    void removerAstronauta(string cpf, int codigo) {
        int a = buscarAstronauta(cpf);
        int v = buscarVoo(codigo);
        if (!conferirAstronauta(a, cpf)) return;
        if (!conferirVoo(v, codigo, "planejado")) return;
        if (!voos[v].removerAstronauta(cpf)) {
            cout << "ERRO: astronauta " << cpf << " nao esta no voo " << codigo << endl;
            return;
        }
        cout << "OK: astronauta " << cpf << " removido do voo " << codigo << endl;
    }
    void lancarVoo(int codigo) {
        int v = buscarVoo(codigo);
        if (!conferirVoo(v, codigo, "planejado")) return;
        if (voos[v].getQuantidadeAstronautas() == 0) {
            cout << "ERRO: voo " << codigo << " nao possui astronautas" << endl;
            return;
        }
        // Primeiro valida toda a tripulacao. Uma falha nao embarca ninguem.
        for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
            string cpf = voos[v].getCpf(i);
            int a = buscarAstronauta(cpf);
            if (!astronautas[a].estaVivo()) {
                cout << "ERRO: astronauta " << cpf << " esta morto" << endl;
                return;
            }
            if (!astronautas[a].estaDisponivel()) {
                cout << "ERRO: astronauta " << cpf << " esta indisponivel" << endl;
                return;
            }
        }
        for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
            int a = buscarAstronauta(voos[v].getCpf(i));
            astronautas[a].embarcar();
        }
        voos[v].lancar();
        cout << "OK: voo " << codigo << " lancado" << endl;
    }
    void finalizarVoo(int codigo) {
        int v = buscarVoo(codigo);
        if (!conferirVoo(v, codigo, "em curso")) return;
        for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
            int a = buscarAstronauta(voos[v].getCpf(i));
            astronautas[a].desembarcar();
        }
        voos[v].finalizar();
        cout << "OK: voo " << codigo << " finalizado com sucesso" << endl;
    }
    void explodirVoo(int codigo) {
        int v = buscarVoo(codigo);
        if (!conferirVoo(v, codigo, "em curso")) return;
        for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
            int a = buscarAstronauta(voos[v].getCpf(i));
            astronautas[a].morrer();
        }
        voos[v].explodir();
        cout << "OK: voo " << codigo << " explodiu" << endl;
    }
    void listarVoos() const {
        string estados[] = {"planejado", "em curso", "finalizado com sucesso",
                            "finalizado com explosao"};
        cout << "LISTA DE VOOS" << endl;
        for (int e = 0; e < 4; e++) {
            cout << "== " << estados[e] << " ==" << endl;
            bool encontrou = false;
            for (int v = 0; v < voos.size(); v++) {
                if (voos[v].getEstado() != estados[e]) continue;
                encontrou = true;
                cout << "Voo " << voos[v].getCodigo() << ": ";
                if (voos[v].getQuantidadeAstronautas() == 0) cout << "sem astronautas";
                for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
                    int a = buscarAstronauta(voos[v].getCpf(i));
                    if (i > 0) cout << ", ";
                    cout << astronautas[a].getCpf() << " " << astronautas[a].getNome();
                }
                cout << endl;
            }
            if (!encontrou) cout << "(nenhum)" << endl;
        }
    }
    void listarMortos() const {
        cout << "ASTRONAUTAS MORTOS" << endl;
        bool encontrou = false;
        for (int a = 0; a < astronautas.size(); a++) {
            if (astronautas[a].estaVivo()) continue;
            encontrou = true;
            string cpf = astronautas[a].getCpf();
            cout << cpf << " " << astronautas[a].getNome() << " - voos:";
            bool participou = false;
            for (int v = 0; v < voos.size(); v++) {
                if (voos[v].getEstado() != "planejado" && voos[v].temAstronauta(cpf)) {
                    cout << " " << voos[v].getCodigo();
                    participou = true;
                }
            }
            if (!participou) cout << " nenhum";
            cout << endl;
        }
        if (!encontrou) cout << "(nenhum)" << endl;
    }
};

int main() {
    Agencia agencia;
    string comando;

    while (cin >> comando) {   // le uma palavra; para no FIM ou quando a entrada acaba
        if (comando == "FIM") {
            break;
        } else if (comando == "CADASTRAR_ASTRONAUTA") {
            string cpf, nome;
            int idade;
            cin >> cpf >> idade;
            getline(cin >> ws, nome);   // o nome vem por ultimo e pode ter espacos
            agencia.cadastrarAstronauta(cpf, nome, idade);
        } else if (comando == "CADASTRAR_VOO") {
            int codigo;
            cin >> codigo;
            agencia.cadastrarVoo(codigo);
        } else if (comando == "ADICIONAR_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            agencia.adicionarAstronauta(cpf, codigo);
        } else if (comando == "REMOVER_ASTRONAUTA") {
            string cpf;
            int codigo;
            cin >> cpf >> codigo;
            agencia.removerAstronauta(cpf, codigo);
        } else if (comando == "LANCAR_VOO") {
            int codigo;
            cin >> codigo;
            agencia.lancarVoo(codigo);
        } else if (comando == "EXPLODIR_VOO") {
            int codigo;
            cin >> codigo;
            agencia.explodirVoo(codigo);
        } else if (comando == "FINALIZAR_VOO") {
            int codigo;
            cin >> codigo;
            agencia.finalizarVoo(codigo);
        } else if (comando == "LISTAR_VOOS") {
            agencia.listarVoos();
        } else if (comando == "LISTAR_MORTOS") {
            agencia.listarMortos();
        } else {
            cout << "ERRO: comando desconhecido " << comando << endl;
        }
    }

    return 0;
}
