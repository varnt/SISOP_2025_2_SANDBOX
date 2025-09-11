class SimpleThread extends Thread {
	//// Aluno Victor Arnt matricula 00291097
	private String name;
	
	public SimpleThread(String name) {
		this.name = name;
	}

	public void run() {
		// Dorme por um tempo aleatório entre 0 e 300 ms
		int sleepTime = (int) (Math.random() * 301);
		try {
			Thread.sleep(sleepTime);
		} catch (InterruptedException ie) {
			ie.printStackTrace();
		}
		// Imprime a mensagem de término e finaliza a thread
		System.out.println("FIM " + name);
	}	
}

public class Main {
	
	public static void main(String args[]) {
		if (args.length < 1) {
			System.out.println("Uso: java Main <numero_de_threads>");
			return; // Sai do programa se nenhum argumento for fornecido
		}

		int t;
		try {
			t = Integer.parseInt(args[0]);
			if (t <= 0) {
				System.out.println("O número de threads deve ser um inteiro positivo.");
				return;
			}
		} catch (NumberFormatException e) {
			System.out.println("Argumento inválido. Por favor, forneça um número inteiro.");
			return; // Sai se o argumento não for um número
		}

		SimpleThread[] threads = new SimpleThread[t];
		for (int i = 0; i < t; i++) {
			threads[i] = new SimpleThread("Thread " + (i + 1));
			threads[i].start();
		}

		for (int i = 0; i < t; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException ie) {
				ie.printStackTrace();
			}
		}
		
		System.out.println("Main finalizando...");
	}
}