/**
 * A classe PingPong atua como um monitor para coordenar duas threads.
 * Ela garante que "ping" e "pong" sejam impressos em alternância.
 */
public class PingPong {
    // Variável de estado compartilhada que define o turno.
    private String turno = "ping";

    /**
     * Imprime "ping", troca o turno para "pong" e notifica outras threads.
     * Este método é sincronizado para garantir exclusão mútua.
     * @param i O número da iteração atual, apenas para fins de depuração/demonstração.
     */
    public synchronized void doPing() {
        // esperar com wait dentro de while quando não for o turno.
        while (!turno.equals("ping")) {
            try {
                wait(); // Libera o monitor e espera.
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Thread Ping interrompida.");
            }
        }

        System.out.println("ping");

        // Troca o turno.
        this.turno = "pong";
        
        // sinalizar a troca de turno com notifyAll.
        notifyAll(); // Acorda as threads que estão esperando.
    }

    /**
     * Imprime "pong", troca o turno para "ping" e notifica outras threads.
     * Este método é sincronizado para garantir exclusão mútua.
     * @param i O número da iteração atual, apenas para fins de depuração/demonstração.
     */
    public synchronized void doPong() {
        // esperar com wait dentro de while quando não for o turno.
        while (!turno.equals("pong")) {
            try {
                wait(); // Libera o monitor e espera.
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Thread Pong interrompida.");
            }
        }

        System.out.println("pong");

        // Troca o turno.
        this.turno = "ping";
        
        // sinalizar a troca de turno com notifyAll.
        notifyAll(); // Acorda as threads que estão esperando.
    }
}