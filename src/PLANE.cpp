
#include "PLANE.h"
#include "Cspline.h"

int PLANE::idx(IDX pos) { return this->sizeX * pos.y() + pos.x(); }

PLANE::PLANE() {
  float x0[17] = {1.0060, 0.9500, 0.9000, 0.8000, 0.7000, 0.6000,
                  0.5000, 0.4000, 0.3000, 0.2000, 0.1500, 0.1000,
                  0.0750, 0.0500, 0.0250, 0.0125, 0.0000};
  float y0[17] = {0.0000, 0.0173, 0.0306, 0.0551, 0.0768, 0.0957,
                  0.1104, 0.1201, 0.1242, 0.1201, 0.1128, 0.1013,
                  0.0933, 0.0825, 0.0666, 0.0558, 0.0337};
  float x1[17] = {0.0000, 0.0125, 0.0250, 0.0500, 0.0750, 0.1000,
                  0.1500, 0.2000, 0.3000, 0.4000, 0.5000, 0.6000,
                  0.7000, 0.8000, 0.9000, 0.9500, 1.0060};
  float y1[17] = {0.0337, 0.0170, 0.0115, 0.0062, 0.0032, 0.0016,
                  0.0003, 0.0000, 0.0005, 0.0015, 0.0024, 0.0030,
                  0.0032, 0.0024, 0.0012, 0.0005, 0.0000};
  this->sp0 = new CSpline;
  this->sp1 = new CSpline;

  // sp0 在上面， sp1 在下面
  for (int i = 16; i >= 0; i--) {
    sp0->push_back(POINT(x0[i] * 100 + 100, y0[i] * 100 + 100));
  }
  for (int i = 0; i < 17; i++) {
    sp1->push_back(POINT(x1[i] * 100 + 100, y1[i] * 100 + 100));
  }

  this->sizeX = 320;
  this->sizeY = 200;
  this->rho = new float[this->sizeX * this->sizeY]{};
  this->vel = new POINT[this->sizeX * this->sizeY]{};
  this->barr = new bool[this->sizeX * this->sizeY]{};
  for (int i = 0; i < this->sizeX * this->sizeY; i++) {
    this->barr[i] = false;
  }
  this->issource_vel = new bool[this->sizeX * this->sizeY]{};
  for (int i = 0; i < this->sizeX * this->sizeY; i++) {
    this->issource_vel[i] = false;
  }
  this->source_vel = new POINT[this->sizeX * this->sizeY]{};

  this->issource_rho = new bool[this->sizeX * this->sizeY]{};
  for (int i = 0; i < this->sizeX * this->sizeY; i++) {
    this->issource_rho[i] = false;
  }

  this->source_rho = new float[this->sizeX * this->sizeY]{};

  for (int x = 0; x < this->sizeX; x++) {
    for (int y = 0; y < this->sizeY; y++) {
      if (y == 0 || y == sizeY - 1) {
        this->barr[idx(IDX(x, y))] = true;
      }
      if (x == 0 || x == 1) {
        this->issource_vel[idx(IDX(x, y))] = true;
        this->source_vel[idx(IDX(x, y))] = VECTOR(U0, 0.);
        this->issource_rho[idx(IDX(x, y))] = true;
        this->source_rho[idx(IDX(x, y))] = D0;
        // this->vel[idx(IDX(x, y))].set(U0, 0.);
      }
      this->vel[idx(IDX(x, y))].set(U0, 0.);
      this->rho[idx(IDX(x, y))] = D0;
    }
  }
}

void PLANE::INIT() {
  IDX pos;
  for (int x = 0; x < sizeX; x++) {
    for (int y = 0; y < sizeY; y++) {
      pos.set(x, y);
      this->vel[idx(pos)].set(U0, 0.);
      this->rho[idx(pos)] = D0;
      if (this->isSourceV(pos) == true) {
        this->vel[idx(pos)] = this->SourceV(pos);
      }
      if (this->isSourceRho(pos) == true) {
        this->mRho(pos, this->sourceRho(pos));
      }
    }
  }
}

IDX PLANE::size() { return IDX(this->sizeX, this->sizeY); }

float PLANE::Rho(IDX pos) { return this->rho[idx(pos)]; }

VECTOR PLANE::V(IDX pos) { return this->vel[idx(pos)]; }

bool PLANE::isBarr(IDX pos) {
  if (pos.x() <= 201 && pos.x() >= 100 &&
      pos.y() <= sp0->get_value((float)pos.x()) &&
      pos.y() >= sp1->get_value((float)pos.x())) {
    return true;
  }
  return this->barr[idx(pos)];
}

bool PLANE::isSourceV(IDX pos) { return this->issource_vel[idx(pos)]; }

VECTOR PLANE::SourceV(IDX pos) { return this->source_vel[idx(pos)]; }

bool PLANE::isSourceRho(IDX pos) { return this->issource_rho[idx(pos)]; }

float PLANE::sourceRho(IDX pos) { return this->source_rho[idx(pos)]; }

void PLANE::mRho(IDX pos, float value) { this->rho[idx(pos)] = value; }

void PLANE::mV(IDX pos, VECTOR value) { this->vel[idx(pos)] = value; }

void PLANE::SAVE() {

  std::ofstream outFile;
  outFile.open("test.csv", std::ios::out);
  for (int i = 0; i < this->sizeX; i++) {
    for (int j = 0; j < this->sizeY; j++) {
      IDX thisPos(i, j);
      // if ((thisPos.x() - 50.) * (thisPos.x() - 50.) +
      //         (thisPos.y() - 50.) * (thisPos.y() - 50.) <
      //     16. * 16.) {
      //   outFile << 0. << ",";
      //   continue;
      // }
      outFile << this->Rho(thisPos) << ",";
    }
    outFile << std::endl;
  }
  outFile.close();

  outFile.open("test_vabs.csv", std::ios::out);
  for (int i = 0; i < this->sizeX; i++) {
    for (int j = 0; j < this->sizeY; j++) {
      IDX thisPos(i, j);
      // if ((thisPos.x() - 50.) * (thisPos.x() - 50.) +
      //         (thisPos.y() - 50.) * (thisPos.y() - 50.) <
      //     16. * 16.) {
      //   outFile << 0. << ",";
      //   continue;
      // }
      outFile << this->V(thisPos).abs() << ",";
    }
    outFile << std::endl;
  }
  outFile.close();

  outFile.open("test_vx.csv", std::ios::out);
  for (int i = 0; i < this->sizeX; i++) {
    for (int j = 0; j < this->sizeY; j++) {
      IDX thisPos(i, j);
      // if ((thisPos.x() - 50.) * (thisPos.x() - 50.) +
      //         (thisPos.y() - 50.) * (thisPos.y() - 50.) <
      //     16. * 16.) {
      //   outFile << 0. << ",";
      //   continue;
      // }
      outFile << this->V(thisPos).x() << ",";
    }
    outFile << std::endl;
  }
  outFile.close();

  outFile.open("test_vy.csv", std::ios::out);
  for (int i = 0; i < this->sizeX; i++) {
    for (int j = 0; j < this->sizeY; j++) {
      IDX thisPos(i, j);
      // if ((thisPos.x() - 50.) * (thisPos.x() - 50.) +
      //         (thisPos.y() - 50.) * (thisPos.y() - 50.) <
      //     16. * 16.) {
      //   outFile << 0. << ",";
      //   continue;
      // }
      outFile << this->V(thisPos).y() << ",";
    }
    outFile << std::endl;
  }
  outFile.close();
}

void PLANE::UPDATE() { return; }
