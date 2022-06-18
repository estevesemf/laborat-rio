//classe da estrutura de dados (recurso) compartilhado entre as threads
class S {
   //recurso compartilhado
   private int r;
   //construtor
   public S() {
      this.r = 0;
   }

   public synchronized void inc() {
      this.r ++;
   }

   public synchronized int get() {
       return this.r;
   }

 }

 //classe que estende Thread e implementa a tarefa de cada thread do programa
 class T extends Thread {
    //identificador da thread
    private int id;
    private int v[];
    private int mod;
    //objeto compartilhado com outras threads
    S s;

    //construtor
    public T(int tid, S s,int v[]) {
       this.id = tid;
       this.s = s;
       this.v = v;
    }

    //metodo main da thread
    public void run() {
       System.out.println("Thread " + this.id + " iniciou!");
       int tamb = (this.v.length) / (par.N) ;
       int ini =id * tamb ;
       int fim = ini + tamb;
       int mod = this.v[id]%2 ;
       //System.out.println("ini " + ini+ " fim " + fim + " v " + this.v[ini]+ " mod " + mod);
       if(id == (par.N - 1)){
          fim = par.N ;
        }
        for(int i=ini; i<fim; i++){
        if(mod==0){
          this.s.inc();
        };
        }
       System.out.println("Thread " + this.id + " terminou!");
    }
 }

 //classe da aplicacao
 class par {
    static final int N = 4;
    static final int tam = 16;
    public static void main (String[] args) {
       //reserva espaço para um vetor de threads
       Thread[] threads = new Thread[N];

       //cria uma instancia do recurso compartilhado entre as threads
       S s = new S();
       int v[] = new int[tam];
       for (int i=0; i<tam; i++) {
          v[i] = i; // na i-ésima posição do vetor "v" armazena o valor da variável "i"
        }

       //cria as threads da aplicacao
       for (int i=0; i<threads.length; i++) {
          threads[i] = new T(i, s, v);
       }

       //inicia as threads
       for (int i=0; i<threads.length; i++) {
          threads[i].start();
       }

       //espera pelo termino de todas as threads
       for (int i=0; i<threads.length; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
       }

       System.out.println("Valor de s = " + s.get());
    }
 }
