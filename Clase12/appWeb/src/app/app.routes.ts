import { Routes } from '@angular/router';
import { Login } from './auth/login/login';

export const routes: Routes = [
  {
    path: "login",
    component: Login,
  },
  {
    path: "auth",
    loadChildren: () => import("./workspace/dashboard/workspace.route").then((m) => m.routes),
  },
  { path: '', redirectTo: 'login', pathMatch: 'full' },
  { path: '**', redirectTo: 'login' },
];
