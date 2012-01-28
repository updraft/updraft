#include "airspace.h"

OpenAirspace::Airspace::Coordinate OpenAirspace::Airspace::X;

namespace OpenAirspace {
  Airspace::Airspace(QTextStream* ts) {
    QString text("");

    // X.valid = false;

    this->validAN = false;
    this->validAL = false;
    this->validAH = false;
    this->validTO = false;
    this->validTC = false;
    // this->validX  = false;
    this->CW      = true;
    this->Wi      = -1;
    // this->validZ  = false;
    this->validDA = false;
    this->validDB = false;
    // float Airspace::Z;
    // this->X.valid = false;
    this->Z = -1;
    this->SP.valid = false;
    this->SB.valid = false;

    (*ts) >> text;
    if (text != "AC") return;

    (*ts) >> text;
    if (text      == "R") this->AC = R;
    else if (text == "Q") this->AC = Q;
    else if (text == "P") this->AC = P;
    else if (text == "A") this->AC = A;
    else if (text == "B") this->AC = B;
    else if (text == "C") this->AC = C;
    else if (text == "D") this->AC = D;
    else if (text == "GP") this->AC = GP;
    else if (text == "CTR") this->AC = CTR;
    else if (text == "W") this->AC = W;
    else
      return;

    while (!(*ts).atEnd()) {
      (*ts) >> text;

      if (text == "AC") return;

      QString parse;
      ts->skipWhiteSpace();
      parse = ts->readLine();

      if (text == "*")
        continue;

      else if (text == "AN") {
        this->AN = parse;
        validAN = true;
      } else if (text == "AL") {
        this->AL = parse;  // !!! can be text e.g. Ask on 122.8 !!!
        validAL = true;
      } else if (text == "AH") {
        this->AH = parse;  // !!! can be text e.g. Ask on 122.8 !!!
        validAH = true;
      } else if (text == "AT") {
        this->AT.push_back(ParseCoord(parse));
      } else if (text == "TO") {
        this->TO = parse;
        validTO = true;
      } else if (text == "TC") {
        this->TC = parse;
        validTC = true;
      } else if (text == "SP") {
        int i = parse.indexOf(',');
        SP.style = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.width = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SP.B = parse.left(i).toInt();
        SP.valid = true;
      } else if (text == "SB") {
        int i = parse.indexOf(',');
        SB.R = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SB.G = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        SB.B = parse.left(i).toInt();
        parse = parse.right(parse.size() - i -1);
        SB.valid = true;
      } else if (text == "V") {
        QChar ch = parse.at(0);
        parse = parse.right(parse.size() - parse.indexOf('=') -1);
        if (ch == 'X') {
          // this->X = ParseCoord(parse);
          X = ParseCoord(parse);
          this->validX = true;
        } else if (ch == 'D') {
          if (parse.trimmed().at(0) == '-')
            this->CW = false;
          else
            this->CW = true;
        } else if (ch == 'W') {
          this->Wi = parse.toInt();
        } else if (ch == 'Z') {
          this->Z = parse.toFloat();
          // this->validZ = true;
        }
      }  /* V */      else if (text == "DP") {
        this->DP.push_back(ParseCoord(parse));
      } else if (text == "DA") {
        // init
        ArcI arc;
        /*if (validZ)*/ arc.Zoom = this->Z;
        /*if (validX)*/ arc.Centre = X;
        arc.CW = this->CW;

        // parse row
        int i = parse.indexOf(',');
        arc.R = parse.left(i).toInt();

        parse = parse.right(parse.size() - i -1);
        i = parse.indexOf(',');
        arc.Start = parse.left(i).toInt();

        parse = parse.right(parse.size() - i -1);
        arc.End = parse.toInt();

        arc.valid = true;
        this->DA.push_back(arc);
      } else if (text == "DB") {
        ArcII arc;
        /*if (validZ)*/ arc.Zoom = this->Z;
        /*if (validX)*/ arc.Centre = X;
        arc.CW = this->CW;

        // parse row
        int i = parse.indexOf(',');
        arc.Start = ParseCoord(parse.left(i));

        parse = parse.right(parse.size() - i -1);

        parse = parse.right(parse.size() - i -1);
        arc.End = arc.Start = ParseCoord(parse);

        arc.valid = true;
        this->DB.push_back(arc);
      } else if (text == "DC") {
        Circle cir;
        /*if (validZ)*/ cir.Zoom = this->Z;
        /*if (validX)*/ cir.Centre = X;

        // parse row
        cir.R = parse.toInt();
        cir.valid = true;
        this->DC.push_back(cir);
      } else if (text == "DY") {
        this->DY.push_back(ParseCoord(parse));
      }
    }
  }

  Airspace::Coordinate Airspace::ParseCoord(const QString& text) {
    Coordinate cor;
    QString parse = text;
    int i = 0, j = 0;
    while (text.at(i) != 'N' && text.at(i) != 'S' && i < text.size()) ++i;
    while (text.at(j) != 'E' && text.at(j) != 'W' && j < text.size()) ++j;
    if (i == text.size() || j == text.size())
      qFatal("Error parsing coordinates in UserAirspace file");
    parse = text.left(i);
    cor.N.min = parse.section(':', 0, 0).toInt();
    cor.N.sec = parse.section(':', 1, 1).toFloat();
    if (text.at(i) == 'S') cor.N.min *= -1;
    parse = text.mid(i+1, j - i - 1);
    cor.E.min = parse.section(':', 0, 0).toInt();
    cor.E.sec = parse.section(':', 1, 1).toFloat();
    if (text.at(j) == 'W') cor.E.min *= -1;
    cor.valid = true;
    return cor;
  }
}  // OpenAirspace
