# Clase 11: Login con Angular 21

En esta clase implementamos una **aplicación web de login** utilizando Angular 21 con Bootstrap y Angular Material. Se exploran conceptos fundamentales de Angular como inyección de dependencias, formularios reactivos y ciclo de vida de componentes.

## 📋 Tabla de Contenidos

- [Requisitos Previos](#requisitos-previos)
- [Crear un Proyecto Angular](#crear-un-proyecto-angular)
- [Estructura del Proyecto](#estructura-del-proyecto)
- [Instalar y Configurar Bootstrap](#instalar-y-configurar-bootstrap)
- [Instalar y Configurar Angular Material](#instalar-y-configurar-angular-material)
- [Conceptos Fundamentales](#conceptos-fundamentales)
- [Ejecutar la Aplicación](#ejecutar-la-aplicación)
- [Descripción del Proyecto](#descripción-del-proyecto)

---

## 🔧 Requisitos Previos

Antes de comenzar, asegúrate de tener instalado:

- **Node.js** (versión 20.x o superior): [Descargar aquí](https://nodejs.org/)
- **npm** (viene con Node.js): Verificar con `npm --version`
- **Angular CLI**: Instalar globalmente con:
  ```bash
  npm install -g @angular/cli@21
  ```

Verificar la instalación:

```bash
ng version
```

### (Recomendado) Instalar NVM en Linux

**NVM** (Node Version Manager) permite gestionar múltiples versiones de Node.js fácilmente. Es especialmente útil cuando trabajas con diferentes proyectos.

#### Instalación de NVM

1. **Descargar e instalar NVM:**

   ```bash
   curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.0/install.sh | bash
   ```

2. **Recargar el bash**:

   ```bash
   source ~/.bashrc
   # O si usas zsh:
   source ~/.zshrc
   ```

3. **Verificar que NVM está instalado:**
   ```bash
   nvm --version
   ```

#### Usar NVM para instalar Node.js

```bash
# Instalar la versión LTS más reciente
nvm install --lts

# Ver versiones instaladas
nvm list

# Cambiar a una versión específica
nvm use 20.11.0

# Establecer una versión por defecto
nvm alias default 20.11.0

# Verificar versión actual
node --version
npm --version
```

**Ventajas de NVM:**

- Cambiar entre versiones de Node.js sin complicaciones
- Proyectos con diferentes versiones de Node.js pueden coexistir
- Fácil actualización y manejo de versiones
- Ideal para desarrollo en equipo con requisitos específicos de versión

---

## 🚀 Crear un Proyecto Angular

### Paso 1: Crear el proyecto

```bash
ng new appWeb
cd appWeb
```

**Opciones recomendadas:**

- Routing: Sí
- Stylesheet format: SCSS
- SSR: No
- Package manager: npm

### Paso 2: Verificar la estructura creada

```
appWeb/
├── src/
│   ├── app/
│   │   ├── app.ts
│   │   ├── app.html
│   │   ├── app.scss
│   │   ├── app.routes.ts
│   │   └── ...
│   ├── index.html
│   ├── main.ts
│   └── styles.scss
├── angular.json
├── package.json
├── tsconfig.json
└── ...
```

---

## 📁 Estructura del Proyecto

```
src/
├── app/
│   ├── app.ts                    # Componente raíz (standalone)
│   ├── app.html                  # Template del componente raíz
│   ├── app.scss                  # Estilos del componente raíz
│   ├── app.routes.ts             # Configuración de rutas
│   ├── app.config.ts             # Configuración global de la app
│   ├── login/
│   │   ├── login.ts              # Componente de login
│   │   ├── login.html            # Template del login
│   │   └── login.scss            # Estilos del login
│   └── shared/
│       └── shared/
│           └── shared-module.ts   # Módulo compartido (imports comunes)
├── index.html                    # HTML principal
├── main.ts                       # Punto de entrada de la app
└── styles.scss                   # Estilos globales
```

### Componentes Standalone

En Angular 21, usamos **componentes standalone** (sin necesidad de NgModule):

```typescript
@Component({
  selector: "app-login",
  imports: [CommonModule, ReactiveFormsModule], // Importar directamente
  templateUrl: "./login.html",
  styleUrl: "./login.scss",
  standalone: true, // Componente standalone
})
export class Login {}
```

---

## 🎨 Instalar y Configurar Bootstrap

### Paso 1: Instalar Bootstrap

```bash
npm install bootstrap@5.3.8
```

### Paso 2: Configurar en `angular.json`

Abre el archivo `angular.json` y en la sección `styles`, agrega Bootstrap:

```json
{
  "projects": {
    "appWeb": {
      "architect": {
        "build": {
          "options": {
            "styles": [
              "node_modules/bootstrap/dist/css/bootstrap.min.css",
              "src/styles.scss"
            ]
          }
        }
      }
    }
  }
}
```

### Paso 3: Usar Bootstrap en tus templates

```html
<!-- Ejemplo: Botón de Bootstrap -->
<button class="btn btn-primary">Login</button>

<!-- Contenedor con clase row y col -->
<div class="container">
  <div class="row">
    <div class="col-md-6">
      <h1>Bienvenido</h1>
    </div>
  </div>
</div>
```

### Paso 4: (Opcional) Importar JavaScript de Bootstrap

Si necesitas componentes interactivos (modales, dropdowns, etc.), agrega a `angular.json`:

```json
"scripts": [
  "node_modules/bootstrap/dist/js/bootstrap.bundle.min.js"
]
```

---

## 🎭 Instalar y Configurar Angular Material

### Paso 1: Instalar Angular Material y CDK

```bash
ng add @angular/material@21
```

Angular CLI preguntará algunas opciones:

- **Color scheme**: Elige uno (ej: `indigo-pink`)
- **Typography**: Sí
- **Animations**: Sí

### Paso 2: Importar módulos en tu componente

```typescript
import { MatInputModule } from "@angular/material/input";
import { MatButtonModule } from "@angular/material/button";
import { MatIconModule } from "@angular/material/icon";
import { MatFormFieldModule } from "@angular/material/form-field";

@Component({
  imports: [MatInputModule, MatButtonModule, MatIconModule, MatFormFieldModule],
  standalone: true,
})
export class MyComponent {}
```

### Paso 3: Usar componentes Material en templates

```html
<!-- Input de Material -->
<mat-form-field appearance="outline">
  <mat-label>Usuario</mat-label>
  <input matInput />
</mat-form-field>

<!-- Botón de Material -->
<button mat-raised-button color="primary">Login</button>

<!-- Icono de Material -->
<mat-icon>visibility</mat-icon>
```

### Módulos Material más comunes

- `MatInputModule` - Inputs avanzados
- `MatButtonModule` - Botones estilizados
- `MatFormFieldModule` - Contenedores de formularios
- `MatIconModule` - Iconos de Material
- `MatCardModule` - Tarjetas
- `MatToolbarModule` - Barras de herramientas
- `MatMenuModule` - Menús
- `MatTableModule` - Tablas

---

## 💡 Conceptos Fundamentales

### 1. **Inject (Inyección de Dependencias)**

#### ¿Qué es Inject?

`inject()` es una función de Angular que permite obtener instancias de servicios y utilidades dentro de un componente. En lugar de pasar dependencias a través del constructor, Angular 21 permite usar `inject()` directamente en el cuerpo del componente.

Angular automáticamente proporciona (inyecta) las instancias necesarias sin que tengas que crearlas manualmente.

#### ¿Por qué es útil?

Normalmente, si necesitaras usar un servicio, tendrías que:

1. Crear una instancia manualmente (`new FormBuilder()`)
2. Pasarla por el constructor
3. Guardarla en una variable

Con `inject()`, Angular se encarga automáticamente.

#### Ejemplo Comparativo

**Manera antigua (con constructor):**

```typescript
@Component({
  selector: "app-login",
  standalone: true,
})
export class Login {
  formBuilder: FormBuilder;

  constructor(fb: FormBuilder) {
    this.formBuilder = fb; // Asignar manualmente
  }
}
```

**Manera moderna (con inject):**

```typescript
import { Component, inject } from "@angular/core";
import { FormBuilder } from "@angular/forms";

@Component({
  selector: "app-login",
  standalone: true,
})
export class Login {
  // Más simple y directo
  formBuilder = inject(FormBuilder);
}
```

#### Cómo funciona inject()

```typescript
// Paso 1: Importar inject
import { Component, inject } from "@angular/core";
import { FormBuilder } from "@angular/forms";

@Component({
  selector: "app-login",
  standalone: true,
})
export class Login {
  // Paso 2: Usar inject() para obtener una instancia de FormBuilder
  formBuilder = inject(FormBuilder);

  // Paso 3: Usar el formBuilder
  ngOnInit() {
    const form = this.formBuilder.group({
      email: ["", Validators.required],
    });
  }
}
```

#### Servicios que puedes inyectar

- `FormBuilder` - Para crear formularios reactivos
- `HttpClient` - Para hacer peticiones HTTP
- `Router` - Para navegación entre rutas
- `ActivatedRoute` - Para acceder a parámetros de ruta
- Tus propios servicios personalizados

**Ventajas:**

- ✅ Código más limpio y legible
- ✅ Menos código boilerplate (constructor)
- ✅ Fácil de testear
- ✅ Desacoplamiento de dependencias
- ✅ Angular gestiona automáticamente las instancias

---

### 2. **Formularios Reactivos**

#### ¿Qué son los formularios reactivos?

Los formularios reactivos son una forma poderosa de manejar formularios en Angular donde:

- El **estado del formulario** se gestiona desde el **componente TypeScript**
- Los campos están conectados a controles (`FormControl`)
- Los validadores se definen en el componente, no en el HTML
- El formulario es **reactivo** (responde a cambios automáticamente)

Es decir, el formulario se controla **programáticamente** desde TypeScript, no desde el HTML.

#### ¿Cómo funciona?

1. **En TypeScript** - Defines la estructura del formulario con validadores
2. **En HTML** - Vinculas los controles del formulario a los inputs
3. **Angular** - Sincroniza automáticamente los datos entre TypeScript e HTML

#### Instalación

```bash
npm install @angular/forms
```

#### Creación del Formulario en TypeScript

```typescript
import { FormBuilder, FormGroup, Validators } from "@angular/forms";
import { Component, inject, OnInit } from "@angular/core";

@Component({
  selector: "app-login",
  standalone: true,
})
export class Login implements OnInit {
  // Inyectar FormBuilder
  formBuilder = inject(FormBuilder);

  // Declarar el formulario
  form!: FormGroup;

  // Se ejecuta cuando el componente se inicializa
  ngOnInit(): void {
    this.createForm();
  }

  // Crear el formulario con estructura y validadores
  createForm(): void {
    this.form = this.formBuilder.group({
      // Nombre del control: [valorInicial, validadores]
      username: ["", [Validators.required, Validators.minLength(3)]],
      password: ["", [Validators.required, Validators.minLength(6)]],
      email: ["", [Validators.required, Validators.email]],
    });
  }

  // Método que se ejecuta cuando el usuario presiona "Enviar"
  login(): void {
    if (this.form.valid) {
      console.log("Usuario:", this.form.value.username);
      console.log("Email:", this.form.value.email);
      console.log("Contraseña:", this.form.value.password);
    } else {
      console.log("Formulario inválido");
    }
  }
}
```

#### Vinculación en el Template HTML

Este es el aspecto más importante: **cómo conectar el formulario TypeScript con los inputs HTML**.

```html
<!-- PASO 1: Vincular el formulario con [formGroup] -->
<form [formGroup]="form" (ngSubmit)="login()">
  <!-- PASO 2: Vincular cada input con formControlName -->
  <div class="form-group">
    <label>Usuario</label>
    <!-- formControlName="username" conecta este input al control 'username' del formulario -->
    <input
      type="text"
      formControlName="username"
      placeholder="Ingresa tu usuario"
      class="form-control"
    />
    <!-- Mostrar error si el campo es inválido -->
    <span *ngIf="form.get('username')?.hasError('required')">
      Username es requerido
    </span>
    <span *ngIf="form.get('username')?.hasError('minlength')">
      Mínimo 3 caracteres
    </span>
  </div>

  <div class="form-group">
    <label>Email</label>
    <input
      type="email"
      formControlName="email"
      placeholder="Ingresa tu email"
      class="form-control"
    />
    <span *ngIf="form.get('email')?.hasError('required')">
      Email es requerido
    </span>
    <span *ngIf="form.get('email')?.hasError('email')"> Email inválido </span>
  </div>

  <div class="form-group">
    <label>Contraseña</label>
    <input
      type="password"
      formControlName="password"
      placeholder="Ingresa tu contraseña"
      class="form-control"
    />
    <span *ngIf="form.get('password')?.hasError('required')">
      Password es requerido
    </span>
    <span *ngIf="form.get('password')?.hasError('minlength')">
      Mínimo 6 caracteres
    </span>
  </div>

  <!-- PASO 3: Botón submit que se deshabilita si el formulario es inválido -->
  <button type="submit" [disabled]="form.invalid" class="btn btn-primary">
    Login
  </button>
</form>
```

#### Explicación paso a paso de la vinculación

**1. `[formGroup]="form"`** - Vincula el elemento `<form>` al objeto `FormGroup` del componente

- Le dice a Angular: "Este formulario HTML corresponde al objeto `form` de TypeScript"
- Todos los inputs dentro deben tener `formControlName`

**2. `formControlName="username"`** - Vincula cada input a un control específico

- Le dice a Angular: "Este input corresponde al control 'username' del formulario"
- **Importante:** El nombre debe coincidir exactamente con la clave en `formBuilder.group()`
- Ejemplos correctos:
  ```html
  formControlName="username"
  <!-- Coincidia con 'username' en group() -->
  formControlName="email"
  <!-- Coincide con 'email' en group() -->
  formControlName="password"
  <!-- Coincide con 'password' en group() -->
  ```

**3. `(ngSubmit)="login()"`** - Ejecuta la función `login()` cuando se envía el formulario

**4. `[disabled]="form.invalid"`** - Deshabilita el botón si el formulario es inválido

- Si cualquier campo tiene errores de validación, el botón se grisea
- El usuario no puede enviar un formulario inválido

#### Acceder a valores del formulario

```typescript
// Obtener todo el objeto del formulario
this.form.value;
// Resultado: { username: 'juan', email: 'juan@example.com', password: '123456' }

// Obtener un control específico
this.form.get("username")?.value; // 'juan'

// Obtener un valor directamente
this.form.controls["username"].value; // 'juan'

// Validar si es válido
this.form.valid; // true si todos los campos son válidos
this.form.invalid; // true si algún campo es inválido

// Ver si el usuario tocó un campo
this.form.get("username")?.touched; // true después de interactuar

// Ver errores de un campo específico
this.form.get("username")?.errors;
// { required: true } o { minlength: { requiredLength: 3, actualLength: 2 } }

// Validar un campo específico
this.form.get("username")?.hasError("required"); // true/false
this.form.get("email")?.hasError("email"); // true/false
```

#### Validadores Comunes

```typescript
// Obligatorio: El campo no puede estar vacío
Validators.required;

// Mínimo de caracteres
Validators.minLength(6); // Mínimo 6 caracteres
Validators.minLength(3); // Mínimo 3 caracteres

// Máximo de caracteres
Validators.maxLength(20); // Máximo 20 caracteres

// Debe coincidir con un patrón (expresión regular)
Validators.pattern(/[a-z]/); // Solo letras minúsculas
Validators.pattern(/^[0-9]{10}$/); // Exactamente 10 dígitos

// Debe ser un email válido
Validators.email; // user@example.com

// Múltiples validadores
username: [
  "",
  [Validators.required, Validators.minLength(3), Validators.maxLength(20)],
];
```

#### Ejemplo completo (TypeScript + HTML)

**login.ts:**

```typescript
import { Component, inject, OnInit } from "@angular/core";
import { FormBuilder, FormGroup, Validators } from "@angular/forms";

@Component({
  selector: "app-login",
  templateUrl: "./login.html",
  standalone: true,
})
export class Login implements OnInit {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;

  ngOnInit(): void {
    this.createForm();
  }

  createForm(): void {
    this.form = this.formBuilder.group({
      username: ["", [Validators.required, Validators.minLength(3)]],
      password: ["", [Validators.required, Validators.minLength(6)]],
    });
  }

  login(): void {
    if (this.form.valid) {
      alert(`Login exitoso! Usuario: ${this.form.value.username}`);
    }
  }
}
```

**login.html:**

```html
<form [formGroup]="form" (ngSubmit)="login()">
  <input type="text" formControlName="username" placeholder="Usuario" />
  <input type="password" formControlName="password" placeholder="Contraseña" />
  <button type="submit" [disabled]="form.invalid">Login</button>
</form>
```

**¿Cómo funciona juntos?**

1. El usuario escribe en el input de username
2. Angular detecta el cambio (porque tiene `formControlName="username"`)
3. Angular actualiza automáticamente `form.value.username`
4. Angular valida según los validadores
5. Si es válido, el botón se habilita
6. Cuando el usuario click en el botón, se ejecuta `login()`
7. Accedemos a los valores con `this.form.value`

---

### 3. **OnInit (Ciclo de Vida)**

#### ¿Qué es ngOnInit?

`ngOnInit` es un **hook del ciclo de vida** (lifecycle hook) de Angular. Es un método especial que se ejecuta **automáticamente** después de que Angular inicializa el componente.

Piensa en él como el "método de startup" del componente - se ejecuta cuando el componente está listo para funcionar.

#### ¿Cuándo se ejecuta?

```
1. Component created (Componente creado)
   ↓
2. Constructor executes (Se ejecuta el constructor)
   ↓
3. Properties initialized (Las propiedades se inicializan)
   ↓
4. ngOnInit() executes ← Aquí es donde entra ngOnInit
   ↓
5. Component ready (Componente listo para usar)
```

#### ¿Por qué es importante?

**❌ No usar ngOnInit (problemático):**

```typescript
@Component({
  selector: "app-login",
  standalone: true,
})
export class Login {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;

  constructor() {
    // ❌ Aquí form aún no está listo
    this.form = this.formBuilder.group({
      username: ["", Validators.required],
    });
    // Puede causar errores porque algunos servicios no están inicializados
  }
}
```

**✅ Usar ngOnInit (correcto):**

```typescript
import { Component, inject, OnInit } from "@angular/core";
import { FormBuilder, FormGroup, Validators } from "@angular/forms";

@Component({
  selector: "app-login",
  standalone: true,
})
export class Login implements OnInit {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;

  // OnInit es llamado cuando el componente está completamente inicializado
  ngOnInit(): void {
    // ✅ Aquí es seguro crear el formulario
    this.createForm();
  }

  createForm(): void {
    this.form = this.formBuilder.group({
      username: ["", Validators.required],
    });
  }
}
```

#### Implementar OnInit

**Paso 1: Importar OnInit**

```typescript
import { Component, OnInit } from "@angular/core";
```

**Paso 2: Implementar la interfaz**

```typescript
@Component({
  selector: "app-login",
  standalone: true,
})
export class Login implements OnInit {
  // ...
}
```

**Paso 3: Escribir el método ngOnInit**

```typescript
export class Login implements OnInit {
  ngOnInit(): void {
    // Código que se ejecuta al inicializar
    console.log("Componente inicializado!");
  }
}
```

#### Casos de uso comunes para ngOnInit

**1. Inicializar formularios**

```typescript
export class Login implements OnInit {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;

  ngOnInit(): void {
    this.form = this.formBuilder.group({
      username: ["", Validators.required],
      password: ["", Validators.required],
    });
  }
}
```

**2. Cargar datos desde una API**

```typescript
export class UserListComponent implements OnInit {
  httpClient = inject(HttpClient);
  users: any[] = [];

  ngOnInit(): void {
    // Obtener lista de usuarios cuando el componente carga
    this.httpClient.get("/api/users").subscribe((data: any) => {
      this.users = data;
    });
  }
}
```

**3. Suscribirse a parámetros de ruta**

```typescript
export class ProfileComponent implements OnInit {
  route = inject(ActivatedRoute);
  userId: string = "";

  ngOnInit(): void {
    // Obtener el ID del usuario de la URL
    this.route.params.subscribe((params) => {
      this.userId = params["id"];
    });
  }
}
```

**4. Configurar valores iniciales**

```typescript
export class DashboardComponent implements OnInit {
  theme = signal("light");

  ngOnInit(): void {
    // Cargar tema guardado
    const savedTheme = localStorage.getItem("theme");
    if (savedTheme) {
      this.theme.set(savedTheme);
    }
  }
}
```

#### Ciclo de vida completo de Angular

Angular tiene varios hooks del ciclo de vida en este orden:

```typescript
import {
  Component,
  OnInit,
  OnChanges,
  OnDestroy,
  AfterViewInit,
} from "@angular/core";

@Component({
  selector: "app-example",
  standalone: true,
})
export class ExampleComponent
  implements OnInit, OnChanges, AfterViewInit, OnDestroy
{
  // 1. Detectar cambios en @Input properties
  ngOnChanges(changes: SimpleChanges): void {
    console.log("Cambios detectados en @Input properties");
  }

  // 2. Inicializar el componente
  ngOnInit(): void {
    console.log("Componente inicializado");
  }

  // 3. Detectar cambios en general
  ngDoCheck(): void {
    console.log("Verificar cambios en el componente");
  }

  // 4. Inicializar vista y contenido
  ngAfterContentInit(): void {
    console.log("Contenido inicializado");
  }

  // 5. Detectar cambios en contenido
  ngAfterContentChecked(): void {
    console.log("Contenido verificado");
  }

  // 6. Inicializar vistas del componente
  ngAfterViewInit(): void {
    console.log("Vistas inicializadas");
  }

  // 7. Detectar cambios en vistas
  ngAfterViewChecked(): void {
    console.log("Vistas verificadas");
  }

  // 8. Limpiar cuando el componente se destruye
  ngOnDestroy(): void {
    console.log("Componente destruido - limpiar recursos");
  }
}
```

**Más comúnmente usados:**

- `ngOnInit` - Inicializar datos, cargar API
- `ngOnDestroy` - Limpiar recursos (suscripciones, timers)
- `ngAfterViewInit` - Acceder a elementos del DOM

#### Ejemplo práctico completo

```typescript
import { Component, OnInit, OnDestroy } from "@angular/core";
import { interval, Subscription } from "rxjs";

@Component({
  selector: "app-timer",
  template: `
    <h1>Tiempo: {{ seconds }}</h1>
    <button (click)="start()">Iniciar</button>
    <button (click)="stop()">Detener</button>
  `,
  standalone: true,
})
export class TimerComponent implements OnInit, OnDestroy {
  seconds = 0;
  subscription?: Subscription;

  ngOnInit(): void {
    console.log("Componente Timer inicializado");
  }

  start(): void {
    // Crear un timer que incrementa cada segundo
    this.subscription = interval(1000).subscribe(() => {
      this.seconds++;
    });
  }

  stop(): void {
    // Detener el timer
    if (this.subscription) {
      this.subscription.unsubscribe();
    }
  }

  ngOnDestroy(): void {
    // ¡IMPORTANTE! Limpiar la suscripción cuando el componente se destruye
    if (this.subscription) {
      this.subscription.unsubscribe();
    }
    console.log("Componente Timer destruido - suscripción cancelada");
  }
}
```

**¿Por qué limpiar en ngOnDestroy?**

- Evitar memory leaks (fugas de memoria)
- Detener peticiones HTTP innecesarias
- Cancelar timers y observables
- Liberar recursos

---

### 4. **Signal (Reactividad moderna)**

#### ¿Qué es un Signal?

Un `Signal` es una forma moderna de Angular 21 para manejar **estado reactivo** en componentes. Es una variable que, cuando cambia, Angular automáticamente actualiza el template.

**Ventajas sobre variables normales:**

- Cambios automáticos se reflejan en el template
- Mejor rendimiento (solo actualiza lo que cambió)
- Código más limpio y legible

#### Comparación: Variable Normal vs Signal

**Variable normal (no reactiva):**

```typescript
export class Login {
  hidePassword: boolean = true; // Variable normal

  togglePassword(): void {
    this.hidePassword = !this.hidePassword;
    // El cambio NO se refleja automáticamente en el template
  }
}
```

**Signal (reactivo):**

```typescript
import { Component, signal } from "@angular/core";

export class Login {
  // Signal: variable reactiva
  hidePassword = signal(true); // Valor inicial: true

  togglePassword(): void {
    // Para cambiar el valor, usamos .set()
    this.hidePassword.set(!this.hidePassword());
    // El cambio se refleja automáticamente en el template
  }
}
```

#### Cómo crear un Signal

```typescript
import { signal } from "@angular/core";

// Sintaxis básica
const miSignal = signal(valorInicial);
```

**Ejemplos:**

```typescript
// Signal con boolean
const isLoading = signal(false);

// Signal con string
const message = signal("Bienvenido");

// Signal con number
const count = signal(0);

// Signal con objeto
const user = signal({ name: "Juan", email: "juan@example.com" });

// Signal con array
const items = signal([1, 2, 3, 4, 5]);
```

#### Operaciones con Signals

**1. Leer el valor:**

```typescript
const isLoading = signal(false);

// Para leer el valor, DEBES llamarlo como función
isLoading(); // false

// ❌ Incorrecto (obtienes la función, no el valor)
console.log(isLoading); // ƒ signal()

// ✅ Correcto
console.log(isLoading()); // false
```

**2. Cambiar el valor:**

```typescript
const isLoading = signal(false);

// Usar .set() para cambiar el valor
isLoading.set(true);
console.log(isLoading()); // true
```

**3. Actualizar basado en el valor anterior:**

```typescript
const count = signal(0);

// Opción 1: Leer y cambiar
count.set(count() + 1);

// Opción 2: Usar .update() (más limpio)
count.update((currentValue) => currentValue + 1);
```

**4. Crear un Signal derivado (computed):**

```typescript
import { signal, computed } from "@angular/core";

const firstName = signal("Juan");
const lastName = signal("Pérez");

// Signal derivado: combina otros signals
const fullName = computed(() => `${firstName()} ${lastName()}`);

console.log(fullName()); // "Juan Pérez"

// Cuando firstName o lastName cambian, fullName se actualiza automáticamente
firstName.set("Carlos");
console.log(fullName()); // "Carlos Pérez"
```

#### Usar Signals en Templates

```typescript
export class Login {
  // Signal
  hidePassword = signal(true);
  count = signal(0);

  // Método para cambiar el signal
  togglePassword(): void {
    this.hidePassword.set(!this.hidePassword());
  }

  increment(): void {
    this.count.update((c) => c + 1);
  }
}
```

**En el HTML:**

```html
<!-- IMPORTANTE: Cuando usas signal en el template, DEBES llamarlo con () -->

<!-- Mostrar el valor del signal -->
<p>Mostrar contraseña: {{ hidePassword() }}</p>

<!-- Mostrar en condicionales -->
<mat-icon *ngIf="hidePassword()">visibility_off</mat-icon>
<mat-icon *ngIf="!hidePassword()">visibility</mat-icon>

<!-- Cambiar el type según el signal -->
<input [type]="hidePassword() ? 'password' : 'text'" placeholder="Contraseña" />

<!-- Ejecutar método al hacer click -->
<button (click)="togglePassword()">
  {{ hidePassword() ? 'Mostrar' : 'Ocultar' }}
</button>

<!-- Mostrar contador -->
<p>Count: {{ count() }}</p>
<button (click)="increment()">Incrementar</button>
```

#### Ejemplo Práctico: Toggle de Visibilidad de Contraseña

**login.ts:**

```typescript
import { Component, signal } from "@angular/core";

@Component({
  selector: "app-login",
  templateUrl: "./login.html",
  standalone: true,
})
export class Login {
  // Signal: controla si la contraseña es visible
  hidePassword = signal(true);

  // Toggle: cambia entre true y false
  togglePasswordVisibility(): void {
    this.hidePassword.set(!this.hidePassword());
  }
}
```

**login.html:**

```html
<div class="form-group">
  <label>Contraseña</label>
  <div class="input-group">
    <!-- El type cambia basado en hidePassword -->
    <input
      [type]="hidePassword() ? 'password' : 'text'"
      placeholder="Ingresa tu contraseña"
      class="form-control"
    />
    <!-- Icono que cambia según el estado -->
    <button
      type="button"
      (click)="togglePasswordVisibility()"
      class="btn btn-outline-secondary"
    >
      <!-- Si hidePassword es true, mostrar "visibility_off", sino "visibility" -->
      {{ hidePassword() ? '👁️‍🗨️ Mostrar' : '👁️ Ocultar' }}
    </button>
  </div>
</div>
```

**¿Qué sucede?**

1. Usuario hace click en el botón
2. Se ejecuta `togglePasswordVisibility()`
3. El signal `hidePassword` cambia de valor
4. Angular detecta el cambio automáticamente
5. El template se actualiza:
   - El `type` del input cambia (password ↔ text)
   - El texto del botón cambia ("Mostrar" ↔ "Ocultar")

#### Signals vs Variables Normales

| Aspecto             | Variable Normal                    | Signal                          |
| ------------------- | ---------------------------------- | ------------------------------- |
| Reactividad         | No automática                      | Automática                      |
| Cambios en template | Requiere `ChangeDetectionStrategy` | Automático                      |
| Performance         | Más lento con muchos cambios       | Más optimizado                  |
| Sintaxis            | `variable = valor`                 | `signal(valor)` y `.set(valor)` |
| Lectura             | `variable`                         | `variable()`                    |
| Modernidad          | Angular clásico                    | Angular 21+                     |

**Cuándo usar Signals:**

- ✅ Estado que afecta el template
- ✅ Valores que cambian frecuentemente
- ✅ Cuando necesitas reactividad
- ❌ No para lógica pura sin representación UI

---

### 5. **SharedModule (Módulo Compartido)**

#### ¿Qué es un SharedModule?

Un **SharedModule** es un contenedor de imports y exports que se reutilizan en múltiples componentes. En lugar de repetir los mismos imports en cada componente, los agrupas en un módulo y lo importas donde lo necesites.

**Problema sin SharedModule:**

```typescript
// login.ts
@Component({
  imports: [
    CommonModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
  ],
  standalone: true,
})
export class Login {}

// register.ts
@Component({
  imports: [
    CommonModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
  ],
  standalone: true,
})
export class Register {}

// dashboard.ts
@Component({
  imports: [
    CommonModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
  ],
  standalone: true,
})
export class Dashboard {}
// ❌ Repetimos lo mismo 3 veces - DRY violation (Don't Repeat Yourself)
```

**Solución con SharedModule:**

```typescript
// Crear una sola vez en shared-module.ts
import { NgModule } from "@angular/core";
import { CommonModule } from "@angular/common";
import { ReactiveFormsModule } from "@angular/forms";
import { MatButtonModule } from "@angular/material/button";
import { MatInputModule } from "@angular/material/input";
import { MatIconModule } from "@angular/material/icon";

@NgModule({
  imports: [
    CommonModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
  ],
  exports: [
    CommonModule,
    ReactiveFormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
  ],
})
export class SharedModule {}
```

**Luego usarlo en componentes:**

```typescript
// login.ts
@Component({
  imports: [SharedModule], // ✅ Un solo import
  standalone: true,
})
export class Login {}

// register.ts
@Component({
  imports: [SharedModule], // ✅ Un solo import
  standalone: true,
})
export class Register {}

// dashboard.ts
@Component({
  imports: [SharedModule], // ✅ Un solo import
  standalone: true,
})
export class Dashboard {}
```

#### Estructura del SharedModule

```
src/app/shared/
├── shared.module.ts         # El módulo compartido
├── components/              # Componentes reutilizables
│   ├── header/
│   │   ├── header.ts
│   │   ├── header.html
│   │   └── header.scss
│   └── footer/
│       ├── footer.ts
│       ├── footer.html
│       └── footer.scss
├── pipes/                   # Pipes personalizados
├── directives/              # Directivas personalizadas
└── services/                # Servicios compartidos
```

#### Ejemplo completo de SharedModule

**shared/shared-module.ts:**

```typescript
import { NgModule } from "@angular/core";
import { CommonModule } from "@angular/common";
import { ReactiveFormsModule, FormsModule } from "@angular/forms";
import { MatButtonModule } from "@angular/material/button";
import { MatInputModule } from "@angular/material/input";
import { MatIconModule } from "@angular/material/icon";
import { MatFormFieldModule } from "@angular/material/form-field";
import { MatCardModule } from "@angular/material/card";

@NgModule({
  // ¿Qué módulos necesita este SharedModule?
  imports: [
    CommonModule, // *ngIf, *ngFor, etc.
    ReactiveFormsModule, // formGroup, formControlName, etc.
    FormsModule, // ngModel, ngForm, etc.
    MatButtonModule, // mat-raised-button, etc.
    MatInputModule, // matInput
    MatIconModule, // mat-icon
    MatFormFieldModule, // mat-form-field
    MatCardModule, // mat-card
  ],
  // ¿Qué exportar para que otros componentes lo usen?
  exports: [
    CommonModule,
    ReactiveFormsModule,
    FormsModule,
    MatButtonModule,
    MatInputModule,
    MatIconModule,
    MatFormFieldModule,
    MatCardModule,
  ],
})
export class SharedModule {}
```

#### Usar SharedModule en un componente

**login.ts:**

```typescript
import { Component, inject, OnInit } from "@angular/core";
import { FormBuilder, FormGroup, Validators } from "@angular/forms";
import { SharedModule } from "../shared/shared-module";

@Component({
  selector: "app-login",
  templateUrl: "./login.html",
  styleUrl: "./login.scss",
  imports: [SharedModule], // ← Importar SharedModule
  standalone: true,
})
export class Login implements OnInit {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;

  ngOnInit(): void {
    this.createForm();
  }

  createForm(): void {
    this.form = this.formBuilder.group({
      username: ["", Validators.required],
      password: ["", Validators.required],
    });
  }

  login(): void {
    console.log("Login:", this.form.value);
  }
}
```

**login.html:**

```html
<!-- Ahora puedo usar todos los módulos del SharedModule -->

<!-- CommonModule: *ngIf -->
<div *ngIf="form">
  <!-- ReactiveFormsModule: formGroup, formControlName -->
  <form [formGroup]="form" (ngSubmit)="login()">
    <!-- MatFormFieldModule: mat-form-field -->
    <mat-form-field appearance="outline">
      <!-- MatInputModule: matInput -->
      <mat-label>Usuario</mat-label>
      <input matInput formControlName="username" />
    </mat-form-field>

    <mat-form-field appearance="outline">
      <mat-label>Contraseña</mat-label>
      <input matInput type="password" formControlName="password" />
    </mat-form-field>

    <!-- MatButtonModule: mat-raised-button -->
    <button
      mat-raised-button
      color="primary"
      type="submit"
      [disabled]="form.invalid"
    >
      <!-- MatIconModule: mat-icon -->
      <mat-icon>login</mat-icon>
      Login
    </button>
  </form>
</div>
```

#### Ventajas del SharedModule

✅ **DRY (Don't Repeat Yourself)** - Escribes imports una sola vez  
✅ **Mantenimiento** - Cambiar imports en un solo lugar  
✅ **Legibilidad** - Código más limpio en componentes  
✅ **Escalabilidad** - Fácil agregar más módulos al compartido  
✅ **Organización** - Estructura clara del proyecto

#### Agregar más cosas al SharedModule

Puedes exportar también componentes y pipes personalizados:

```typescript
// shared/shared-module.ts
import { NgModule } from "@angular/core";
import { CommonModule } from "@angular/common";
import { HeaderComponent } from "./components/header/header";
import { FooterComponent } from "./components/footer/footer";
import { TruncatePipe } from "./pipes/truncate.pipe";

@NgModule({
  imports: [
    CommonModule,
    HeaderComponent, // Componente standalone
    FooterComponent,
    TruncatePipe, // Pipe personalizado
  ],
  exports: [CommonModule, HeaderComponent, FooterComponent, TruncatePipe],
})
export class SharedModule {}
```

Luego en el login puedes usar:

```html
<!-- Usar componente del SharedModule -->
<app-header></app-header>

<form [formGroup]="form">
  <!-- ... -->
</form>

<!-- Usar pipe del SharedModule -->
<p>{{ text | truncate: 20 }}</p>

<app-footer></app-footer>
```

---

## ▶️ Ejecutar la Aplicación

### Iniciar el servidor de desarrollo

Desde la carpeta del proyecto (`appWeb`):

```bash
cd appWeb
npm start
```

O equivalentemente usando Angular CLI:

```bash
ng serve
```

Para abrir automáticamente el navegador:

```bash
ng serve -o
```

**Salida esperada:**

```
✔ Compiled successfully.
Application bundle generated successfully.

Local:        http://localhost:4200/
Browser started on http://localhost:4200/
```

### Acceder a la aplicación

Abre tu navegador y ve a:

```
http://localhost:4200
```

Deberías ver el formulario de login en pantalla.

### Entender la salida del servidor

```
✔ Compiled successfully.  ← ¡Sin errores!
0 errors
18 warnings
```

- **errors**: Problemas de sintaxis que detienen la compilación
- **warnings**: Advertencias (código funciona pero hay mejoras posibles)

### Hot Module Replacement (HMR)

Angular detecta cambios automáticamente:

1. **Haces cambios** en cualquier archivo (`.ts`, `.html`, `.scss`)
2. **Angular compila** automáticamente los cambios
3. **El navegador actualiza** automáticamente (sin recargar la página completa)
4. **Estado se mantiene** (datos en memoria se preservan)

Esto se llama **Hot Module Replacement** o **HMR** - es super útil para desarrollo.

### Detener el servidor

En la terminal donde corre el servidor, presiona:

```bash
Ctrl + C
```

El mensaje será algo como:

```
^C
Closing the terminal will also close any open processes.
Press any key to continue...
```

### Otros comandos útiles

```bash
# Compilar para producción (optimizado para el navegador)
npm run build
# Genera una carpeta 'dist/' con los archivos minificados

# Ejecutar tests unitarios (pruebas del código)
npm test
# Abre una interfaz donde puedes ver los tests pasar/fallar

# Compilar y vigilar cambios sin abrir navegador
npm run watch

# Ver versión de Angular CLI
ng version

# Generar un nuevo componente
ng generate component nombre-componente
# O más corto:
ng g c nombre-componente

# Generar un nuevo servicio
ng g s nombre-servicio

# Ver lista de comandos disponibles
ng --help
```

### Solucionar problemas comunes

**Error: "ng: command not found"**

```bash
# Solución: Instalar Angular CLI globalmente
npm install -g @angular/cli@21
```

**Error: "Cannot find module..."**

```bash
# Solución: Reinstalar dependencias
npm install
```

**Puerto 4200 ya está en uso**

```bash
# Solución: Usar otro puerto
ng serve --port 4300
```

**Componentes no se actualizan en el navegador**

```bash
# Solución: Limpiar caché y recompilar
rm -rf dist/  # En Linux/Mac
rmdir /s dist  # En Windows
npm start
```

````

---

## 📝 Descripción del Proyecto

### Componente App (Raíz)

[src/app/app.ts](src/app/app.ts) - Componente principal que importa el componente de login.

```typescript
@Component({
  selector: 'app-root',
  imports: [RouterOutlet, SharedModule, Login],
  standalone: true
})
export class App {
  title = signal('appWeb');
}
````

### Componente Login

[src/app/login/login.ts](src/app/login/login.ts) - Formulario de login con:

- **Formulario reactivo** con campos username y password
- **Validadores** para campos obligatorios
- **Signal** para toggle de visibilidad de contraseña
- **Método login()** que imprime los valores del formulario

```typescript
@Component({
  selector: "app-login",
  imports: [SharedModule],
  templateUrl: "./login.html",
  styleUrl: "./login.scss",
  standalone: true,
})
export class Login implements OnInit {
  formBuilder = inject(FormBuilder);
  form!: FormGroup;
  hide = signal(true);

  ngOnInit(): void {
    this.createForm();
  }

  createForm(): void {
    this.form = this.formBuilder.group({
      username: ["", Validators.required],
      password: ["", Validators.required],
    });
  }

  login(): void {
    console.log("Usuario:", this.form.value.username);
    console.log("Contraseña:", this.form.value.password);
  }

  clickEvent(event: MouseEvent): void {
    this.hide.set(!this.hide());
    event.stopPropagation();
  }
}
```

### Template del Login

[src/app/login/login.html](src/app/login/login.html) - Interfaz de usuario del formulario con Bootstrap y Material.

### Estilos

[src/app/login/login.scss](src/app/login/login.scss) - Estilos específicos del componente login.

---

## 📚 Recursos Útiles

- [Documentación oficial de Angular](https://angular.dev)
- [Bootstrap Documentation](https://getbootstrap.com/docs)
- [Angular Material Components](https://material.angular.io)
- [Formularios Reactivos en Angular](https://angular.dev/guide/forms/reactive-forms)
- [Inyección de Dependencias](https://angular.dev/guide/di)

---

**Autor:** Clase 11 - Sistemas Operacionales 2 - Diego Juarez
**Fecha:** Diciembre 2025
