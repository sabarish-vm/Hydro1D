#pragma once
#include <exception>
#include <stdexcept>
#include <memory>

#include "./Cell.hpp"
#include "Bondi.hpp"
#include "OptionNames.hpp"

namespace BCs {
template <BoundaryType BTYPE>
void apply_bcs(std::unique_ptr<Cell[]>& cells, const uint32_t& ncell) {
  if constexpr (BTYPE == BoundaryType::OPEN) {
    cells[0]._rho = cells[1]._rho;
    cells[0]._u = cells[1]._u;
    cells[0]._P = cells[1]._P;

    cells[ncell + 1]._rho = cells[ncell]._rho;
    cells[ncell + 1]._u = cells[ncell]._u;
    cells[ncell + 1]._P = cells[ncell]._P;
  } else if constexpr (BTYPE == BoundaryType::BONDI) {
    BondiFunc::boundary_conditions_initialize(cells, ncell);
  } else if constexpr (BTYPE == BoundaryType::REFLECTIVE) {
        std::runtime_error("REFLECTIVE boundary conditions not implemented yet");
  }
}
}  // namespace BCs

