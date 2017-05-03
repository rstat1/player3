import { Component, OnInit } from '@angular/core';
import { NgbModule, NgbDropdown } from '@ng-bootstrap/ng-bootstrap';

@Component({
  selector: 'nav-box',
  templateUrl: './html/page-switcher.html',
  styleUrls: ['./css/page-switcher.scss']
})
export class PageSwitcherComponent {

  AppRoutes = [
    {itemName: 'Games', path: "directory/games"},
    {itemName: 'Discover', path: 'directory'},
    {itemName: 'Now Playing', path: ''},
  ]
  constructor() { }
  public nav(path) {
  }

}
