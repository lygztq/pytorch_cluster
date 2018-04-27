#include <torch/torch.h>


at::Tensor graclus(at::Tensor row, at::Tensor col, at::Tensor weight) {
  return row;
}


at::Tensor grid(at::Tensor pos, at::Tensor size, at::Tensor start, at::Tensor end) {
  size = size.toType(pos.type());
  start = start.toType(pos.type());
  end = end.toType(pos.type());

  pos = pos - start.view({ 1, -1 });
  auto num_voxels = ((end - start) / size).toType(at::kLong);
  num_voxels = (num_voxels + 1).cumsum(0);
  num_voxels = num_voxels - num_voxels[0];
  num_voxels[0] = 1;

  auto cluster = pos / size.view({ 1, -1 });
  cluster = cluster.toType(at::kLong);
  cluster *= num_voxels.view({ 1, -1 });
  cluster = cluster.sum(1);

  return cluster;
}


PYBIND11_MODULE(TORCH_EXTENSION_NAME, m) {
  m.def("graclus", &graclus, "Graclus (CPU)");
  m.def("grid", &grid, "Grid (CPU)");
}