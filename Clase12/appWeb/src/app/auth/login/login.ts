import { Component, inject, OnInit, signal } from '@angular/core';
import { SharedModule } from '../../shared/shared-module';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';
import { Auth, ResponseLogin } from '../auth';
import { Router } from '@angular/router';
import { Storage } from '../../shared/service/storage';

@Component({
  selector: 'app-login',
  imports: [SharedModule],
  templateUrl: './login.html',
  styleUrl: './login.scss',
  standalone: true,
})
export class Login implements OnInit{

  formBuilder = inject(FormBuilder);
  form!: FormGroup;
  authService = inject(Auth)
  router = inject(Router);
  storageService = inject(Storage);


  ngOnInit(): void {
    this.createForm()
  }

  createForm(){
    this.form = this.formBuilder.group({
      username: ['', Validators.required],
      password: ['', Validators.required],
    })
  }


  async login(){
    console.log('user ', this.form.value.username)
    console.log('password ', this.form.value.password)
    if(this.form.valid){
      try {
        const response: ResponseLogin = await this.authService.authLogin({username: this.form.value.username, password: this.form.value.password})
        this.storageService.setUser(response)
        console.log(response)
        await this.router.navigate(['auth/workspace'])
      } catch (error: any) {
        if(error.status === 401){
          alert("Usuario o contraseña inconrrectos")
        }else{
          console.log('Error inesperado: ' + error.message);
        }
      }
    }
  }

 hide = signal(true);
  clickEvent(event: MouseEvent) {
    this.hide.set(!this.hide());
    event.stopPropagation();
  }
}
