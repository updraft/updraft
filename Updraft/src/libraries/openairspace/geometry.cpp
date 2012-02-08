#include "geometry.h"

namespace OpenAirspace {

Polygon::Polygon() {
  this->centre = new Position();
  this->valid = false;
}
Polygon::Polygon(const Position& C_, const double& z_) {
  this->centre = new Position(C_);
  this->zoom = z_;
  this->valid = true;
}
Polygon::~Polygon() {
  delete this->centre;
  this->centre = NULL;
}
ArcI::ArcI() {
  this->centre = new Position();
  this->valid = false;
}
ArcI::ArcI(const Position& C_, const double& R_, const bool CW_,
  const double& S_, const double& E_, const double& z_) {
  this->centre = new Position(C_);
  this->r = R_;
  this->cw = CW_;
  this->start = S_;
  this->end = E_;
  this->zoom = z_;
  this->valid = true;
}
ArcI::~ArcI() {
  delete this->centre;
  this->centre = NULL;
}
ArcII::ArcII() {
  this->centre = new Position();
  this->start = new Position();
  this->end = new Position();
  this->valid = false;
}
ArcII::ArcII(const Position& c_, const Position& s_, const Position e_,
  const bool cw_, const double& z_) {
  this->centre = new Position(c_);
  this->start = new Position(s_);
  this->end = new Position(e_);
  this->cw = cw_;
  this->zoom = z_;
  this->valid = true;
}
ArcII::~ArcII() {
  delete this->centre;
  delete this->start;
  delete this->end;
  this->centre = this->start = this->end = NULL;
}
Circle::Circle() {
  this->centre = new Position();
  this->valid = false;
}
Circle::Circle(const OpenAirspace::Position &c_,
  const double R_, const double &z_) {
  this->centre = new Position(c_);
  this->r = R_;
  this->zoom = z_;
  this->valid = true;
}
Circle::~Circle() {
  delete this->centre;
  this->centre = NULL;
}
}  // OpenAirspace
