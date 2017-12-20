#include <ctype.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "visualdl/logic/sdk.h"

namespace py = pybind11;
namespace vs = visualdl;

PYBIND11_PLUGIN(core) {
  py::module m("core", "C++ core of VisualDL");
  //  m.doc() = "visualdl python core API";

  py::class_<vs::TabletHelper>(m, "Tablet")
      // other member setter and getter
      .def("record_buffer", &vs::TabletHelper::record_buffer)
      .def("records_size", &vs::TabletHelper::records_size)
      .def("buffer", &vs::TabletHelper::buffer)
      .def("human_readable_buffer", &vs::TabletHelper::human_readable_buffer)
      .def("set_buffer",
           (void (vs::TabletHelper::*)(const std::string&)) &
               vs::TabletHelper::SetBuffer)
      // scalar interface
      .def("as_int32_scalar",
           [](vs::TabletHelper& self, vs::ImHelper& im) {
             return vs::components::ScalarHelper<int32_t>(self, &im.handler());
           })
      .def("as_int64_scalar",
           [](vs::TabletHelper& self, vs::ImHelper& im) {
             return vs::components::ScalarHelper<int64_t>(&self.data(),
                                                          &im.handler());
           })
      .def("as_float_scalar",
           [](vs::TabletHelper& self, vs::ImHelper& im) {
             return vs::components::ScalarHelper<float>(&self.data(),
                                                        &im.handler());
           })
      .def("as_double_scalar", [](vs::TabletHelper& self, vs::ImHelper& im) {
        return vs::components::ScalarHelper<double>(&self.data(),
                                                    &im.handler());
      });

  py::class_<vs::StorageHelper>(m, "Storage")
      .def("timestamp", &vs::StorageHelper::timestamp)
      .def("dir", &vs::StorageHelper::dir)
      .def("set_dir", &vs::StorageHelper::SetDir)
      .def("tablets_size", &vs::StorageHelper::tablets_size)
      .def("buffer", &vs::StorageHelper::buffer)
      .def("human_readable_buffer", &vs::StorageHelper::human_readable_buffer)
      .def("set_buffer",
           (void (vs::StorageHelper::*)(const std::string&)) &
               vs::StorageHelper::SetBuffer);

  py::class_<vs::ImHelper>(m, "Im")
      .def("__init__",
           [](vs::ImHelper& instance) { new (&instance) vs::ImHelper(); })
      .def("storage", &vs::ImHelper::storage)
      .def("tablet", &vs::ImHelper::tablet)
      .def("add_tablet", &vs::ImHelper::AddTablet)
      .def("persist_to_disk", &vs::ImHelper::PersistToDisk)
      .def("clear_tablets", &vs::ImHelper::ClearTablets)
      .def("start_read_service",
           &vs::ImHelper::StartReadService,
           "start a thread to maintain read service")
      .def("start_write_service",
           &vs::ImHelper::StartWriteSerice,
           "start a thread to maintain write service")
      .def("stop_service",
           &vs::ImHelper::StopService,
           "stop the service thread");

// interfaces for components begin

// different data type of scalar conponent
#define ADD_SCALAR_TYPED_INTERFACE(T, name__)                             \
  py::class_<vs::components::ScalarHelper<T>>(m, #name__)                 \
      .def("add_record", &vs::components::ScalarHelper<T>::AddRecord)     \
      .def("set_captions", &vs::components::ScalarHelper<T>::SetCaptions) \
      .def("get_records", &vs::components::ScalarHelper<T>::GetRecords)   \
      .def("get_captions", &vs::components::ScalarHelper<T>::GetCaptions) \
      .def("get_ids", &vs::components::ScalarHelper<T>::GetIds)           \
      .def("get_record_size", &vs::components::ScalarHelper<T>::GetSize)  \
      .def("get_timestamps", &vs::components::ScalarHelper<T>::GetTimestamps);
  ADD_SCALAR_TYPED_INTERFACE(int32_t, ScalarInt32);
  ADD_SCALAR_TYPED_INTERFACE(int64_t, ScalarInt64);
  ADD_SCALAR_TYPED_INTERFACE(float, ScalarFloat);
  ADD_SCALAR_TYPED_INTERFACE(double, ScalarDouble);
#undef ADD_SCALAR_TYPED_INTERFACE
}