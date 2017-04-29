import { MdGridListModule, MdButtonModule } from '@angular/material';
import { NgModule } from "@angular/core";

@NgModule({
	imports: [MdGridListModule, MdButtonModule],
	exports: [MdGridListModule, MdButtonModule],
})
export class MaterialImports {}