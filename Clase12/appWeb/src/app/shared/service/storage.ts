import { computed, Injectable, signal } from '@angular/core';
import { ResponseLogin } from '../../auth/auth';

@Injectable({
  providedIn: 'root',
})
export class Storage {
  user = signal<ResponseLogin | null>(this.readUser());
  isLoggedIn = computed(() => this.user !== null);

  private readUser(): ResponseLogin | null {
    const raw = localStorage.getItem('user');
    try {
      return raw ? (JSON.parse(raw) as ResponseLogin) : null;
    } catch (error) {
      return null
    }
  }


  setUser(user: ResponseLogin){
    localStorage.setItem('user', JSON.stringify(user));
    this.user.set(user)
  }

  clearUser(){
    localStorage.removeItem('user');
    this.user.set(null);
  }

  deleteAllStorege(){
    localStorage.clear()
    this.user.set(null)
  }


  
}
