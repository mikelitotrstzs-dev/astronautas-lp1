#include <iostream>
#include <string>
#include <vector>
#include <fstream>

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

    // Este metodo so e chamado na agencia temporaria de carregar().
    bool lerDados(istream& arquivo) {
        string versao;
        int quantidade;
        if (!(arquivo >> versao) || versao != "ASTRONAUTAS_V1") return false;
        if (!(arquivo >> quantidade) || quantidade < 0) return false;
        for (int i = 0; i < quantidade; i++) {
            string cpf, nome;
            int idade, vivo, disponivel;
            if (!(arquivo >> cpf)) return false;
            if (!getline(arquivo >> ws, nome) || nome.empty()) return false;
            if (!(arquivo >> idade >> vivo >> disponivel)) return false;
            if ((vivo != 0 && vivo != 1) || (disponivel != 0 && disponivel != 1)) return false;
            if ((!vivo && disponivel) || buscarAstronauta(cpf) != -1) return false;
            Astronauta astronauta(cpf, nome, idade);
            if (!vivo) astronauta.morrer();
            else if (!disponivel) astronauta.embarcar();
            astronautas.push_back(astronauta);
        }
        if (!(arquivo >> quantidade) || quantidade < 0) return false;
        for (int i = 0; i < quantidade; i++) {
            int codigo, tripulantes;
            string estado;
            if (!(arquivo >> codigo) || buscarVoo(codigo) != -1) return false;
            if (!getline(arquivo >> ws, estado)) return false;
            if (estado != "planejado" && estado != "em curso" &&
                estado != "finalizado com sucesso" && estado != "finalizado com explosao") return false;
            if (!(arquivo >> tripulantes) || tripulantes < 0) return false;
            if (estado != "planejado" && tripulantes == 0) return false;
            Voo voo(codigo);
            for (int j = 0; j < tripulantes; j++) {
                string cpf;
                if (!(arquivo >> cpf) || buscarAstronauta(cpf) == -1 || voo.temAstronauta(cpf)) return false;
                voo.adicionarAstronauta(cpf);
            }
            if (estado == "em curso") voo.lancar();
            else if (estado == "finalizado com sucesso") voo.finalizar();
            else if (estado == "finalizado com explosao") voo.explodir();
            voos.push_back(voo);
        }
        // Confere as relacoes: ninguem pode estar em dois voos em curso.
        for (int a = 0; a < astronautas.size(); a++) {
            int emCurso = 0;
            for (int v = 0; v < voos.size(); v++) {
                if (!voos[v].temAstronauta(astronautas[a].getCpf())) continue;
                if (voos[v].getEstado() == "em curso") emCurso++;
                if (voos[v].getEstado() == "finalizado com explosao" && astronautas[a].estaVivo()) return false;
            }
            if (emCurso > 1 || (!astronautas[a].estaVivo() && emCurso > 0)) return false;
            bool disponivel = astronautas[a].estaVivo() && emCurso == 0;
            if (astronautas[a].estaDisponivel() != disponivel) return false;
        }
        arquivo >> ws;
        return arquivo.eof() && !arquivo.bad();
    }

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
    void listarAstronautas() const {
        string grupos[] = {"disponiveis", "em voo", "mortos"};
        cout << "LISTA DE ASTRONAUTAS" << endl;
        for (int g = 0; g < 3; g++) {
            cout << "== " << grupos[g] << " ==" << endl;
            bool encontrou = false;
            for (int a = 0; a < astronautas.size(); a++) {
                int grupo = 2;
                if (astronautas[a].estaVivo()) {
                    grupo = astronautas[a].estaDisponivel() ? 0 : 1;
                }
                if (grupo != g) continue;
                encontrou = true;
                string cpf = astronautas[a].getCpf();
                cout << cpf << " " << astronautas[a].getNome()
                     << " (" << astronautas[a].getIdade() << " anos)";
                if (g == 1) {
                    for (int v = 0; v < voos.size(); v++) {
                        if (voos[v].getEstado() == "em curso" && voos[v].temAstronauta(cpf)) {
                            cout << " - voo " << voos[v].getCodigo();
                            break;
                        }
                    }
                }
                cout << endl;
            }
            if (!encontrou) cout << "(nenhum)" << endl;
        }
    }
    void historico(string cpf) const {
        int a = buscarAstronauta(cpf);
        if (!conferirAstronauta(a, cpf)) return;
        cout << "HISTORICO DE " << cpf << " " << astronautas[a].getNome() << endl;
        bool encontrou = false;
        for (int v = 0; v < voos.size(); v++) {
            if (voos[v].getEstado() != "planejado" && voos[v].temAstronauta(cpf)) {
                cout << "voo " << voos[v].getCodigo() << ": " << voos[v].getEstado() << endl;
                encontrou = true;
            }
        }
        if (!encontrou) cout << "(nenhum voo)" << endl;
    }
    void salvar(string nomeArquivo) const {
        ofstream arquivo(nomeArquivo.c_str());
        if (arquivo) {
            arquivo << "ASTRONAUTAS_V1" << endl << astronautas.size() << endl;
            for (int a = 0; a < astronautas.size(); a++) {
                arquivo << astronautas[a].getCpf() << endl
                        << astronautas[a].getNome() << endl
                        << astronautas[a].getIdade() << " " << astronautas[a].estaVivo()
                        << " " << astronautas[a].estaDisponivel() << endl;
            }
            arquivo << voos.size() << endl;
            for (int v = 0; v < voos.size(); v++) {
                arquivo << voos[v].getCodigo() << endl << voos[v].getEstado() << endl
                        << voos[v].getQuantidadeAstronautas() << endl;
                for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
                    arquivo << voos[v].getCpf(i) << endl;
                }
            }
            arquivo.close();
        }
        if (!arquivo) cout << "ERRO: nao foi possivel salvar em " << nomeArquivo << endl;
        else cout << "OK: dados salvos em " << nomeArquivo << endl;
    }
    void carregar(string nomeArquivo) {
        ifstream arquivo(nomeArquivo.c_str());
        Agencia temporaria;
        if (!arquivo || !temporaria.lerDados(arquivo)) {
            cout << "ERRO: nao foi possivel carregar de " << nomeArquivo << endl;
            return;
        }
        astronautas = temporaria.astronautas;
        voos = temporaria.voos;
        cout << "OK: dados carregados de " << nomeArquivo << endl;
    }
    void relatorio() const {
        int planejados = 0, emCurso = 0, sucessos = 0, explosoes = 0;
        for (int v = 0; v < voos.size(); v++) {
            string estado = voos[v].getEstado();
            if (estado == "planejado") planejados++;
            else if (estado == "em curso") emCurso++;
            else if (estado == "finalizado com sucesso") sucessos++;
            else if (estado == "finalizado com explosao") explosoes++;
        }
        int vivos = 0, maisExperiente = -1, maiorExperiencia = 0;
        for (int a = 0; a < astronautas.size(); a++) {
            if (astronautas[a].estaVivo()) vivos++;
            int experiencia = 0;
            for (int v = 0; v < voos.size(); v++) {
                if (voos[v].getEstado() != "planejado" &&
                    voos[v].temAstronauta(astronautas[a].getCpf())) experiencia++;
            }
            // Nao troca em empate: conserva o primeiro cadastrado.
            if (experiencia > maiorExperiencia) {
                maiorExperiencia = experiencia;
                maisExperiente = a;
            }
        }
        cout << "RELATORIO" << endl;
        cout << "voos planejados: " << planejados << endl;
        cout << "voos em curso: " << emCurso << endl;
        cout << "voos finalizados com sucesso: " << sucessos << endl;
        cout << "voos finalizados com explosao: " << explosoes << endl;
        cout << "astronautas cadastrados: " << astronautas.size() << endl;
        cout << "astronautas vivos: " << vivos << endl;
        cout << "astronautas mortos: " << astronautas.size() - vivos << endl;
        cout << "astronauta mais experiente: ";
        if (maisExperiente == -1) cout << "(nenhum)" << endl;
        else {
            cout << astronautas[maisExperiente].getCpf() << " "
                 << astronautas[maisExperiente].getNome()
                 << " (voos lancados: " << maiorExperiencia << ")" << endl;
        }
        cout << "taxa de sucesso: ";
        int finalizados = sucessos + explosoes;
        if (finalizados == 0) cout << "(nenhum voo finalizado)" << endl;
        else cout << sucessos * 100LL / finalizados << "%" << endl;
    }
    void listarTripulacao(int codigo) const {
        int v = buscarVoo(codigo);
        if (v == -1) {
            cout << "ERRO: voo " << codigo << " nao cadastrado" << endl;
            return;
        }
        cout << "TRIPULACAO DO VOO " << codigo << endl;
        cout << "estado: " << voos[v].getEstado() << endl;
        if (voos[v].getQuantidadeAstronautas() == 0) cout << "(nenhum)" << endl;
        for (int i = 0; i < voos[v].getQuantidadeAstronautas(); i++) {
            int a = buscarAstronauta(voos[v].getCpf(i));
            string situacao = "morto";
            if (astronautas[a].estaVivo()) {
                situacao = astronautas[a].estaDisponivel() ? "disponivel" : "em voo";
            }
            cout << astronautas[a].getCpf() << " " << astronautas[a].getNome()
                 << " (" << astronautas[a].getIdade() << " anos) - " << situacao << endl;
        }
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
        } else if (comando == "LISTAR_ASTRONAUTAS") {
            agencia.listarAstronautas();
        } else if (comando == "HISTORICO") {
            string cpf;
            cin >> cpf;
            agencia.historico(cpf);
        } else if (comando == "SALVAR") {
            string arquivo;
            cin >> arquivo;
            agencia.salvar(arquivo);
        } else if (comando == "CARREGAR") {
            string arquivo;
            cin >> arquivo;
            agencia.carregar(arquivo);
        } else if (comando == "RELATORIO") {
            agencia.relatorio();
        } else if (comando == "TRIPULACAO") {
            int codigo;
            cin >> codigo;
            agencia.listarTripulacao(codigo);
        } else {
            cout << "ERRO: comando desconhecido " << comando << endl;
        }
    }

    return 0;
}
