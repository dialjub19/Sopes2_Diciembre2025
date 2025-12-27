import { Component, signal } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { SharedModule } from './shared/shared-module';
import {Login} from './auth/login/login'


@Component({
  selector: 'app-root',
  imports: [RouterOutlet, SharedModule, Login],
  templateUrl: './app.html',
  styleUrl: './app.scss',
  standalone: true,
})
export class App {
  protected readonly title = signal('appWeb');
}
