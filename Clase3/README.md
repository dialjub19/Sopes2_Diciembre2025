# Guía para Go en Linux

## Instalación de Go en Linux

1. Descarga la última versión de Go desde la página oficial: https://go.dev/dl/
2. Descomprime el archivo descargado:
   ```sh
   tar -C /usr/local -xzf go1.xx.x.linux-amd64.tar.gz
   ```
3. Agrega Go al PATH. Añade esto a tu archivo abriendolo con nano o gedit `~/.bashrc` o `~/.profile`, si usas zsh como bash entonces añade en `~/.zshrc`:
   ```sh
   gedit ~/.bashrc
   export PATH=$PATH:/usr/local/go/bin
   ```
4. Aplica los cambios:

   ```sh
   source ~/.bashrc

   // si usas zsh
   source ~/.zshrc
   ```

5. Verifica la instalación:
   ```sh
   go version
   ```

## Iniciar un proyecto en Go

1. Crea una carpeta para tu proyecto y entra en ella:
   ```sh
   mkdir mi-proyecto && cd mi-proyecto
   ```
2. Inicializa el módulo:
   ```sh
   go mod init ejemplo.com/mi-proyecto
   ```
   Esto crea el archivo `go.mod` que gestiona las dependencias del proyecto.

## ¿Qué es el archivo go.mod?

El archivo `go.mod` define el módulo principal del proyecto y sus dependencias. Permite reproducir el entorno de desarrollo y facilita la gestión de versiones de paquetes externos.

## ¿Qué son las gorutinas?

Las gorutinas son funciones o métodos que se ejecutan de manera concurrente en Go. Se lanzan usando la palabra clave `go` antes de la llamada a la función. Son ligeras y gestionadas por el runtime de Go, permitiendo miles de gorutinas en un solo programa.

Ejemplo:

```go
go miFuncion()
```

## ¿Qué son los canales?

Los canales (`chan`) permiten la comunicación y sincronización entre gorutinas. Se usan para enviar y recibir datos de manera segura entre diferentes gorutinas.

### ¿Qué es `make` y por qué se usa con los canales?

La función `make` en Go se utiliza para crear canales, slices y mapas. En el caso de los canales, `make` reserva el espacio necesario y devuelve un canal listo para usarse. No se puede usar un canal sin antes crearlo con `make`, ya que de lo contrario el programa generará un error.

Ejemplo:

```go
canal := make(chan int)       // Crea un canal de enteros sin búfer
canalBuf := make(chan int, 5) // Crea un canal de enteros con búfer de tamaño 5
```

Se usa `make` porque los canales son estructuras internas gestionadas por el runtime de Go, y necesitan ser inicializadas correctamente antes de usarse para enviar o recibir datos.

Ejemplo:

```go
canal := make(chan int)
go func() { canal <- 42 }()
valor := <-canal
```

Así, los canales ayudan a coordinar el trabajo concurrente en Go.

### ¿Qué es un búfer y para qué sirve en los canales?

Un búfer en un canal es una capacidad interna que permite almacenar un número limitado de valores enviados al canal antes de que sea necesario que otra gorutina los reciba. Un canal sin búfer (o de búfer 0) obliga a que el envío y la recepción ocurran al mismo tiempo (comunicación sincronizada). Un canal con búfer permite que varias gorutinas envíen datos sin que otra los reciba inmediatamente, hasta que el búfer se llena.

Ejemplo:

```go
canal := make(chan int, 3) // Canal con búfer de tamaño 3
canal <- 1 // Se almacena en el búfer
canal <- 2 // Se almacena en el búfer
canal <- 3 // Se almacena en el búfer
// Si intentas enviar otro valor, el envío se bloqueará hasta que alguien reciba un valor
```

El uso de búfer es útil para desacoplar la velocidad de producción y consumo de datos entre gorutinas.

### ¿Cómo se debe cerrar un canal correctamente?

Un canal debe cerrarse usando la función `close(canal)` únicamente cuando ya no se van a enviar más datos por ese canal. Solo el remitente (productor) debe cerrar el canal, nunca el receptor (consumidor). Cerrar un canal permite a las gorutinas receptoras detectar que no habrá más datos (por ejemplo, usando un bucle `for valor := range canal`).

Ejemplo:

```go
canal := make(chan int)
go func() {
    for i := 0; i < 5; i++ {
        canal <- i
    }
    close(canal) // Se cierra el canal cuando ya no se enviarán más datos
}()
for valor := range canal {
    fmt.Println(valor)
}
```

No es obligatorio cerrar un canal a menos que los receptores necesiten saber que no habrá más datos.

### ¿Cómo debe terminar una gorutina correctamente?

Una gorutina debe terminar cuando su función finaliza. Es importante evitar que una gorutina quede bloqueada esperando datos de un canal que nunca llegarán, o intentando enviar datos a un canal que nadie recibirá. Para una terminación ordenada, se pueden usar canales para notificar la finalización, o asegurarse de que todas las operaciones de envío/recepción tengan correspondencia.

Ejemplo de gorutina que termina correctamente:

```go
func trabajador(done chan bool) {
    // Realiza trabajo...
    done <- true // Notifica que terminó
}

func main() {
    done := make(chan bool)
    go trabajador(done)
    <-done // Espera a que la gorutina termine
}
```

## 🔗 Links de referencias

- [goroutines&channels](https://okanexe.medium.com/concurrency-with-goroutines-and-channels-in-go-b6e8bace6d94)
- [goroutines-visually](https://blog.stackademic.com/go-concurrency-visually-explained-select-statement-b546596c8e6b)

