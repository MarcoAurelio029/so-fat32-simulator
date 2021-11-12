///  +--------------------------------------+  ///
///  |         Sistemas Operacionais        |  ///
///  |  Atividade 3 - Alocação de Arquivos  |  ///
///  |  UFC Russas - Prof. Daniel Siqueira  |  ///
///  +--------------------------------------+  ///
///  |  Nome: Marco Aurelio Lima  - 418852  |  ///
///  +--------------------------------------+  ///

#include <iostream>
#include <vector>

using namespace std;

// Struct que representa dodo o armazenamento de dados
struct disco_t
{
    int qtd_blocos;
    int tam_blocos;

    string bytes;
    vector<int>    tabela_fat;
    vector<string> tabela_dir;
    vector<int>    tabela_dir_bloco;
};

void print_disco   (struct disco_t& disco);
void print_sistema (struct disco_t& disco);
void criar_arquivo (struct disco_t& disco);
void gravar_arquivo(struct disco_t& disco);
void ler_arquivo   (struct disco_t& disco);
void apagar_arquivo(struct disco_t& disco);
void inserir_final_arquivo(struct disco_t& disco);

void print_disco(struct disco_t& disco)
{
    // Todo o código dessa função é para apresentar os dados da struct disco_t de forma organizada

    cout<<"+--------------------------------------+"<<endl;
    cout<<"|         Sistemas Operacionais        |"<<endl;
    cout<<"|  Atividade 3 - Alocação de Arquivos  |"<<endl;
    cout<<"|  UFC Russas - Prof. Daniel Siqueira  |"<<endl;
    cout<<"+--------------------------------------+"<<endl;
    cout<<"|  Nome: Marco Aurelio Lima  - 418852  |"<<endl;
    cout<<"+--------------------------------------+"<<endl;

    cout << "\n ARMAZENAMENTO: [" << disco.qtd_blocos << " blocos de " << disco.tam_blocos << " bytes]\n ";

    unsigned tam_linha = (disco.tam_blocos+3) * 8;
    if(tam_linha < 47){ tam_linha = 47; }

    for(unsigned i=0;i<tam_linha;i++) { cout<<"-"; } cout<<endl;

    cout << " disco:\n [";

    for(unsigned i=0;i<disco.bytes.size();i++)
    {
        cout << disco.bytes[i];

        if(i%(disco.tam_blocos * 8)==(disco.tam_blocos * 8)-1)
        {
            if(i != disco.bytes.size()-1) { cout<<"]\n ["; continue; }
            else { cout<<"]\n "; }
        }
        else
        {
            if(i%(unsigned)disco.tam_blocos==(unsigned)disco.tam_blocos-1){ cout<< "] ["; }
        }
    }


    cout << "\n tabela FAT:\n  ";
    for(unsigned i=0;i<disco.tabela_fat.size();i++)
    {
        if(disco.tabela_fat[i] == -2)
        {
            cout << i << "[ ]";

            if(i+1 <= 9){ cout<< "    "; } else
            if(i+1 >= 10 && i+1 <= 99){ cout<< "   "; }
        } else
        {
            cout << i << "[" << disco.tabela_fat[i] << "]";
            if(i+1 <= 9)
            {
                if(disco.tabela_fat[i] < 0)
                {
                    cout<< "   ";
                } else
                if(disco.tabela_fat[i] <= 9 && disco.tabela_fat[i] >= 0)
                {
                    cout<< "    ";
                } else
                if(disco.tabela_fat[i] >= 10 && disco.tabela_fat[i] <= 99)
                {
                    cout<< "   ";
                }
            } else
            if(i+1 >= 10 && i+1 <= 99)
            {
                if(disco.tabela_fat[i] < 0)
                {
                    cout<< "  ";
                } else
                if(disco.tabela_fat[i] <= 9 && disco.tabela_fat[i] >= 0)
                {
                    cout<< "  ";
                } else
                if(disco.tabela_fat[i] >= 10 && disco.tabela_fat[i] <= 99)
                {
                    cout<< "  ";
                }
            }
        }

        if(i%8 == 7){
            if(i<=9){ cout<<endl<<"  "; }
            else{ cout<<endl<<" "; }
        }
    }

    cout << "\n\n tabela de diretorio:\n";
    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        cout << " ["<< i << "] " << disco.tabela_dir[i] << "  (" << disco.tabela_dir_bloco[i] << ")\n";
    }

    cout<<endl<<" ";
    for(unsigned i=0;i<tam_linha;i++) { cout<<"-"; } cout<<endl;
}


void print_sistema(struct disco_t& disco)
{

    cout<<" | [1] Criar   [2] Apagar  [3] Ler  [4]  Gravar |\n";
    cout<<" +----------------------------------------------+\n";
    cout<<" > ";
    string c = "";
    cin >> c;

    if(c == "1")
    {
        criar_arquivo(disco);
    } else if(c == "2")
    {
        apagar_arquivo(disco);
    } else if(c == "3")
    {
        ler_arquivo(disco);
    } else if(c == "4")
    {
        gravar_arquivo(disco);
    } else if(c == "5")
    {
        inserir_final_arquivo(disco);
    }

}


void criar_arquivo(struct disco_t& disco)
{
    ///1.1. Pede um nome de arquivo
    ///1.2. Cria um registro na tabela do diretório com esse nome

    system("clear");
    print_disco(disco);

    cout<<"\n [criar arquivo]\n";

    cout<<" (1/1) nome: ";
    string nome;
    cin >> nome;

    // Verificar se foi digitado "..". se for, retornar da função e voltar ao menu.
    if(nome == ".."){ return; }

    // Verificar se já existe arquivo com mesmo nome
    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        if(disco.tabela_dir[i] == nome)
        {
            cout<< "\n [erro] Já existe um arquivo com o mesmo nome neste local.\n";
            cout<< "\n Pressione uma tecla...";
            getchar();
            getchar();
            return;
        }
    }

    // Colocar string em 'tabela_dir'
    disco.tabela_dir.push_back(nome);

    // Colocar valor '-1' 'em tabela_dir_bloco'
    // O '-1' representa um bloco nulo, pois o arquivo ainda é vazio e nao ocupa espaço.
    disco.tabela_dir_bloco.push_back(-1);

}


void gravar_arquivo(struct disco_t& disco)
{

    system("clear");
    print_disco(disco);

    cout<<"\n [gravar no arquivo]\n";

    cout<<" (1/2) nome: ";
    string nome;
    cin >> nome;

    // Verificar se foi digitado "..". se for, retornar da função e voltar ao menu.
    if(nome == ".."){ return; }

    // Verificar se este arquivo já existe na tabela de diretorios
    int arq_index = -1;

    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        if(disco.tabela_dir[i] == nome)
        {
            arq_index = i;
            break;
        }
    }

    if(arq_index == -1)
    {
        cout<<"\n [!] Arquivo NAO encontrado.\n";
        cout<< "\n Pressione uma tecla...";

        getchar();
        getchar();
        return;
    }

    // Verificar se arquivo já tem conteudo gravado
    // Se tiver, será preciso colocar -1
    if(disco.tabela_dir_bloco[arq_index] != -1)
    {
        cout<< "\n [!] Este arquivo já contem dados. Deseja sobrescrever? [s/n] ";
        string res;
        cin >> res;

        if(res == "n" || res == "N")
        {
            return;
        }
        else
        {
            int bloco = disco.tabela_dir_bloco[arq_index];

            while(bloco != -1)
            {   int index = bloco;
                bloco = disco.tabela_fat[bloco];
                disco.tabela_fat[index] = -2;
            }
        }
    }


    // Pegar o conteudo a ser guadado no arquivo
    cout << " (2/2) conteudo: ";

    getchar();
    string conteudo;
    std::getline(std::cin, conteudo);

    // Alocar blocos para guardar onteudo do arquivo
    int qtd_blocos = 0;

    // Calcular quantidade de blocos necessarios para guardar conteudo
    if(conteudo.size() % (unsigned)disco.tam_blocos == 0)
    {
        qtd_blocos = conteudo.size() / (unsigned)disco.tam_blocos;
    } else
    {
        qtd_blocos = (conteudo.size() / (unsigned)disco.tam_blocos) + 1;
    }

    // Armazenar sequencia de blocos disponoveis
    vector<int> blocos_disp;
    for(unsigned i=0;i<disco.tabela_fat.size();i++)
    {
        // Encontrou um bloco disponivel
        if(disco.tabela_fat[i] == -2)
        {
           blocos_disp.push_back(i);
           qtd_blocos--;
        }

        if(qtd_blocos == 0){ break; }
    }
    // Inserir -1 na ultima posicão
    blocos_disp.push_back(-1);

    // Reservar os blocos encontrados preenchendo com indice do proximo bloco
    for(unsigned i=0;i<blocos_disp.size()-1;i++)
    {
        disco.tabela_fat[blocos_disp[i]] = blocos_disp[i+1];
    }

    // Gravar primeiro bloco do arquivo na tabela de diretorio
    disco.tabela_dir_bloco[arq_index] = blocos_disp[0];

    // Preencher o disco seguindo a sequenvia da tabela FAT
    // Pegar indice do primeiro bloco na tabela de diretorios
    int bloco = disco.tabela_dir_bloco[arq_index];

    for(unsigned i=0;i<conteudo.size();i++)
    {
        // Calcular o indice do caractere para o array do disco seguindo os blocos da tabela FAT
        int index = (bloco * disco.tam_blocos) + (i%disco.tam_blocos);

        if(i%disco.tam_blocos == (unsigned)(disco.tam_blocos-1)){
            // quando acaba um bloco, chama-se o proximo
            bloco = disco.tabela_fat[bloco];
        }
        // colocar o caractere no indice calculado
        disco.bytes[index] = conteudo[i];

        // Preencher espaços que sobraram do ultimo bloco com ' '; (espacos vazios)
        if(i==conteudo.size()-1)
        {
            int qtd_sobra = conteudo.size()%(unsigned)disco.tam_blocos;

            if(qtd_sobra == 0){ break; }

            //printf("sobra: %d\n", qtd_sobra);

            for(int j=0;j<disco.tam_blocos-qtd_sobra;j++)
            {
                // colocar o caractere no indice calculado
                disco.bytes[index+1+j] = ' ';
            }
        }
    }

}


void ler_arquivo(struct disco_t& disco)
{
    system("clear");
    print_disco(disco);

    cout<<"\n [ler arquivo]\n";

    cout<<" (1/1) nome: ";
    string nome;
    cin >> nome;

    // Verificar se foi digitado "..". se for, retornar da função e voltar ao menu.
    if(nome == ".."){ return; }

    // Verificar se este arquivo já existe na tabela de diretorios
    int arq_index = -1;

    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        if(disco.tabela_dir[i] == nome)
        {
            arq_index = i;
            break;
        }
    }

    if(arq_index == -1)
    {
        cout<<"\n [!] Arquivo NAO encontrado.\n";
        cout<< "\n Pressione uma tecla...";

        getchar();
        getchar();
        return;
    }

    cout << "\n ";

    // Exibir conteudo usando os blocos da tabela FAT
    int bloco = disco.tabela_dir_bloco[arq_index];

    while(bloco != -1)
    {
        int index = (bloco * disco.tam_blocos);

        for(int i=0;i<disco.tam_blocos;i++)
        {
            cout<< disco.bytes[index+i];
        }

        bloco = disco.tabela_fat[bloco];
    }

    cout<< "\n\n Pressione uma tecla...";
    getchar();
    getchar();
}


void apagar_arquivo(struct disco_t& disco)
{
    system("clear");
    print_disco(disco);

    cout<<"\n [apagar arquivo]\n";
    cout<<" (1/1) nome: ";
    string nome;
    cin >> nome;

    // Verificar se foi digitado "..". se for, retornar da função e voltar ao menu.
    if(nome == ".."){ return; }

    // Verificar se este arquivo já existe na tabela de diretorios
    int arq_index = -1;

    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        if(disco.tabela_dir[i] == nome)
        {
            arq_index = i;
            break;
        }
    }

    if(arq_index == -1)
    {
        cout<<"\n [!] Arquivo NAO encontrado.\n";
        cout<< "\n Pressione uma tecla...";

        getchar();
        getchar();
        return;
    }

    // Desalocar espaço em disco; (inserir '.')
    int bloco = disco.tabela_dir_bloco[arq_index];

    while(bloco != -1)
    {
        int index = (bloco * disco.tam_blocos);

        for(int i=0;i<disco.tam_blocos;i++)
        {
            disco.bytes[index+i] = '.';
        }

        bloco = disco.tabela_fat[bloco];
    }

    // Desalocar arquivo da tabela FAT
    bloco = disco.tabela_dir_bloco[arq_index];

    while(bloco != -1)
    {   int index = bloco;
        bloco = disco.tabela_fat[bloco];
        disco.tabela_fat[index] = -2;
    }

    disco.tabela_dir.erase(disco.tabela_dir.begin() + arq_index);
    disco.tabela_dir_bloco.erase(disco.tabela_dir_bloco.begin() + arq_index);


}


void inserir_final_arquivo(struct disco_t& disco)
{
    system("clear");
    print_disco(disco);

    cout<<"\n [inserir no final do arquivo]\n";

    cout<<" (1/1) nome: ";
    string nome;
    cin >> nome;

    // Verificar se foi digitado "..". se for, retornar da função e voltar ao menu.
    if(nome == ".."){ return; }

    // Verificar se este arquivo já existe na tabela de diretorios
    int arq_index = -1;

    for(unsigned i=0;i<disco.tabela_dir.size();i++)
    {
        if(disco.tabela_dir[i] == nome)
        {
            arq_index = i;
            break;
        }
    }

    if(arq_index == -1)
    {
        cout<<"\n [!] Arquivo NAO encontrado.\n";
        cout<< "\n Pressione uma tecla...";

        getchar();
        getchar();
        return;
    }

    /// Falta continuar

}


int main()
{

    int tam_blocos = 0;
    int qtd_blocos = 0;

    while(tam_blocos == 0 || qtd_blocos == 0)
    {
        cout<<"+--------------------------------------+"<<endl;
        cout<<"|         Sistemas Operacionais        |"<<endl;
        cout<<"|  Atividade 3 - Alocação de Arquivos  |"<<endl;
        cout<<"|  UFC Russas - Prof. Daniel Siqueira  |"<<endl;
        cout<<"+--------------------------------------+"<<endl;
        cout<<"|  Nome: Marco Aurelio Lima  - 418852  |"<<endl;
        cout<<"+--------------------------------------+"<<endl;

        cout<<"(1/2) Tamanho dos blocos: ";
        cin >> tam_blocos;

        cout<<"(2/2) Quantidade de blocos: ";
        cin >> qtd_blocos;

        system("clear");
    }

    string bytes = "";
    for(int i=0;i<(qtd_blocos * tam_blocos);i++){ bytes += '.'; }

    // -1 representa fim do arquivo e -2 representa blocos disponiveis
    vector<int> tabela_fat;
    for(int i=0;i<qtd_blocos;i++){ tabela_fat.push_back(-2); }

    // Declarar uma struct do tipo disco_t
    struct disco_t disco;
    disco.qtd_blocos = qtd_blocos;
    disco.tam_blocos = tam_blocos;
    disco.bytes      = bytes;
    disco.tabela_fat = tabela_fat;


    for(;;)
    {
        print_disco(disco);
        print_sistema(disco);

        system("clear");
    }

    return 0;
}
