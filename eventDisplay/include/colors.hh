#include "G4Colour.hh"
#include "DetectorConstruction.hh"

void setHGCALHitColor (VisHit* hit) {
      float _r, _g, _b;
      float hit_energy = hit->energy;
      if (hit_energy < 3.5) {_r = 0; _g = 0; _b = 1;}
      else if (hit_energy < 50.) {_r = 0; _g = 0.5; _b = 0.5;}
      else if (hit_energy < 100.) {_r = 0; _g = 1; _b = 0;}
      else if (hit_energy < 150.) {_r = 0.5; _g = 0.5; _b = 0;}
      else if (hit_energy < 200.) {_r = 0.8; _g = 0.2; _b = 0;}
      else if (hit_energy < 300.) {_r = 1.0; _g = 0.; _b = 0;}
      else if (hit_energy < 500.) {_r = 0.5; _g = 0.; _b = 0.5;}
      else {_r = 0.; _g = 0.; _b = 0.;}
      hit->red = _r;
      hit->green = _g;
      hit->blue = _b;
}

void setAHCALHitColor (VisHit* hit) {
      float _r, _g, _b;
      float hit_energy = hit->energy;
      if (hit_energy < 3.5) {_r = 0; _g = 0; _b = 1;}
      else if (hit_energy < 50.) {_r = 0; _g = 0.5; _b = 0.5;}
      else if (hit_energy < 100.) {_r = 0; _g = 1; _b = 0;}
      else if (hit_energy < 150.) {_r = 0.5; _g = 0.5; _b = 0;}
      else if (hit_energy < 200.) {_r = 0.8; _g = 0.2; _b = 0;}
      else if (hit_energy < 300.) {_r = 1.0; _g = 0.; _b = 0;}
      else if (hit_energy < 500.) {_r = 0.5; _g = 0.; _b = 0.5;}
      else {_r = 0.; _g = 0.; _b = 0.;}
      hit->red = _r;
      hit->green = _g;
      hit->blue = _b;
}