import { Component, inject, OnInit, signal } from '@angular/core';
import { SharedModule } from '../shared/shared/shared-module';
import { FormBuilder, FormGroup, Validators } from '@angular/forms';

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

  ngOnInit(): void {
    this.createForm()
  }

  createForm(){
    this.form = this.formBuilder.group({
      username: ['', Validators.required],
      password: ['', Validators.required],
    })
  }


  login(){
    console.log('user ', this.form.value.username)
    console.log('password ', this.form.value.password)
  }

 hide = signal(true);
  clickEvent(event: MouseEvent) {
    this.hide.set(!this.hide());
    event.stopPropagation();
  }
}
