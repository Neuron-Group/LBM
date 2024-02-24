
// 复杂与无限细节的神
// 我为你写下颂歌
// 愿你神圣温柔的春天降临

#include "LBM.h"

int LBM::idx(int dir, IDX pos) { return (pos.y() * NX + pos.x()) * 9 + dir; }

void LBM::NX_update() { this->NX = pl.size().x(); }

void LBM::NY_update() { this->NY = pl.size().y(); }

void LBM::N_update() {
  this->NX = pl.size().x();
  this->NY = pl.size().y();
}

float LBM::feq(int dir, float rho, VECTOR u) {
  float eu, uv, ans;
  eu = this->e[dir] * u;
  uv = u * u;
  ans = w[dir] * rho * (1.0 + 3.0 * eu + 4.5 * eu * eu - 1.5 * uv);
  return ans;
}

void LBM::INIT() {
  this->N_update();
  this->pl.INIT();

  IDX pos;
  for (int x = 0; x < NX; x++) {
    for (int y = 0; y < NY; y++) {
      pos.set(x, y);
      for (int k = 0; k < 9; k++) {
        this->f[idx(k, pos)] = this->feq(k, pl.Rho(pos), pl.V(pos));
      }
    }
  }
}

LBM::LBM(PLANE &pl_) : pl(pl_) {
  this->Re = 2000.;
  this->niu = 50. / 2000.;
  this->rt = 3. * niu + 0.5;

  IDX e_[9] = {IDX(0, 0), IDX(1, 0),  IDX(0, 1),   IDX(-1, 0), IDX(0, -1),
               IDX(1, 1), IDX(-1, 1), IDX(-1, -1), IDX(1, -1)};

  for (int k = 0; k < 9; k++) {
    this->E[k] = e_[k];
    this->e[k] = e_[k].IDX2POINT();
  }

  float w_[9] = {4.0 / 9.,  1.0 / 9.,  1.0 / 9.,  1.0 / 9., 1.0 / 9.,
                 1.0 / 36., 1.0 / 36., 1.0 / 36., 1.0 / 36.};

  this->f = new float[pl.size().x() * pl.size().y() * 9]{};
  this->F = new float[pl.size().x() * pl.size().y() * 9]{};

  for (int k = 0; k < 9; k++) {
    this->w[k] = w_[k];
  }
  INIT();
}

void LBM::FLUID_FIELD() {
  IDX pos, pos_;
  int x, y, k;
  for (x = 0; x < NX; x++) {
    for (y = 0; y < NY; y++) {
      pos.set(x, y);
      for (k = 0; k < 9; k++) {
        F[idx(k, pos)] = f[idx(k, pos)] +
                         (feq(k, pl.Rho(pos), pl.V(pos)) - f[idx(k, pos)]) / rt;
      }
    }
  }

  for (x = 1; x < NX - 1; x++) {
    for (y = 1; y < NY - 1; y++) {
      pos.set(x, y);
      for (k = 0; k < 9; k++) {
        pos_ = pos - E[k];
        // ip = x - e[k][0];
        // jp = y - e[k][1];
        f[idx(k, pos)] = F[idx(k, pos_)];
      }
    }
  }
}

void LBM::MACRO() {
  IDX pos;
  int x, y, k;
  float rho_;
  VECTOR vel_, v_;
  for (x = 0; x < NX; x++) {
    for (y = 0; y < NY; y++) {
      pos.set(x, y);
      rho_ = 0.;
      vel_.set(0., 0.);
      for (k = 0; k < 9; k++) {
        rho_ = rho_ + f[idx(k, pos)];
        v_ = f[idx(k, pos)] * e[k];
        vel_ = vel_ + v_;
      }
      pl.mRho(pos, rho_);
      if (dcmp(rho_) != 0) {
        pl.mV(pos, vel_ / rho_);
      }
    }
  }
}

void LBM::FLUID_BOUNDARY() {
  IDX pos, pos1, pos2, pos3, pos4;
  int x, y, k, sum;
  VECTOR O(0., 0.);
  float rho_, f_[9];
  for (x = 1; x < NX - 1; x++) {
    for (y = 1; y < NY - 1; y++) {
      if (pl.isBarr(pos) == true) {
        continue;
      }
      pos.set(x, y);
      pos1.set(x - 1, y);
      pos2.set(x + 1, y);
      pos3.set(x, y - 1);
      pos4.set(x, y + 1);
      rho_ = 0.;
      sum = 0;
      if (pl.isBarr(pos1) == true) {
        sum++;
        rho_ += pl.Rho(pos2);
      }
      if (pl.isBarr(pos2) == true) {
        sum++;
        rho_ += pl.Rho(pos1);
      }
      if (pl.isBarr(pos3) == true) {
        sum++;
        rho_ += pl.Rho(pos4);
      }
      if (pl.isBarr(pos4) == true) {
        sum++;
        rho_ += pl.Rho(pos3);
      }
      if (sum > 0) {
        pl.mRho(pos, rho_ / (float)sum);
      }
    }
  }

  for (x = 1; x < NX - 1; x++) {
    for (y = 1; y < NY - 1; y++) {
      pos.set(x, y);
      if (pl.isBarr(pos) == true) {
        pl.mV(pos, O);
        // pl.mRho(pos, 0.);
        // std::cout << pos.x() << " " << pos.y() << std::endl;
        continue;
      }
      pos1.set(x - 1, y);
      pos2.set(x + 1, y);
      pos3.set(x, y - 1);
      pos4.set(x, y + 1);
      sum = 0;
      memset(f_, 0, sizeof(float) * 9);
      if (pl.isBarr(pos1) == true) {
        sum++;
        for (k = 0; k < 9; k++) {
          f_[k] += feq(k, pl.Rho(pos), pl.V(pos)) +
                   (f[idx(k, pos2)] - feq(k, pl.Rho(pos2), pl.V(pos2)));
        }
      }
      if (pl.isBarr(pos2) == true) {
        sum++;
        for (k = 0; k < 9; k++) {
          f_[k] += feq(k, pl.Rho(pos), pl.V(pos)) +
                   (f[idx(k, pos1)] - feq(k, pl.Rho(pos1), pl.V(pos1)));
        }
      }
      if (pl.isBarr(pos3) == true) {
        sum++;
        for (k = 0; k < 9; k++) {
          f_[k] += feq(k, pl.Rho(pos), pl.V(pos)) +
                   (f[idx(k, pos4)] - feq(k, pl.Rho(pos4), pl.V(pos4)));
        }
      }
      if (pl.isBarr(pos4) == true) {
        sum++;
        for (k = 0; k < 9; k++) {
          f_[k] += feq(k, pl.Rho(pos), pl.V(pos)) +
                   (f[idx(k, pos3)] - feq(k, pl.Rho(pos3), pl.V(pos3)));
        }
      }
      if (sum > 0) {
        for (int k = 0; k < 9; k++) {
          f[idx(k, pos)] = f_[k] / (float)sum;
        }
      }
    }
  }

  for (x = 0; x < NX; x++) {
    for (y = 0; y < NY; y++) {
      pos.set(x, y);
      if (pl.isBarr(pos) == true) {
        continue;
      }
      if (pl.isSourceV(pos)) {
        pl.mV(pos, pl.SourceV(pos));
      }
      if (pl.isSourceRho(pos)) {
        pl.mRho(pos, pl.sourceRho(pos));
      }
    }
  }
}

void LBM::RUN_ONE_STEP() {
  // N_update();
  std::cout << "Calculation of distribution function...";
  FLUID_FIELD();
  std::cout << "Done." << std::endl;
  std::cout << "Calculation of speed and density...";
  MACRO();
  std::cout << "Done." << std::endl;
  std::cout << "Handling of boundary conditions...";
  FLUID_BOUNDARY();
  std::cout << "Done." << std::endl;
  std::cout << "Updating domain...";
  this->pl.UPDATE();
  std::cout << "Done." << std::endl;
}

void LBM::RUN_MULTI_STEP(int T) {
  for (int i = 1; i <= T; i++) {
    RUN_ONE_STEP();
    std::cout << ">> STEP     : " << i << std::endl;
    std::cout << ">> PROGRESS : " << 100. * (float)i / (float)T << " %"
              << std::endl;
  }
}

void LBM::SAVE() { pl.SAVE(); }
