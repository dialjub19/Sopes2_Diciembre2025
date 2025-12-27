import { HttpClient } from '@angular/common/http';
import { inject, Injectable } from '@angular/core';
import { lastValueFrom } from 'rxjs';

@Injectable({
  providedIn: 'root',
})
export class Auth {
  private base = 'http://localhost:18080';
  http = inject(HttpClient);

  async authLogin(login: AuthLogin){
    return lastValueFrom(this.http.post<ResponseLogin>(this.base + "/auth", login));
  }
  
}

export interface AuthLogin {
  username: string;
  password: string;
}

export interface ResponseLogin {
  ok: boolean;
  role: string;
  username: string;
}