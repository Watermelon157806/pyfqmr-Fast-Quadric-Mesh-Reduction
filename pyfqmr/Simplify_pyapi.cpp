#include "Simplify.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>

namespace py = pybind11;

namespace Simplify {
    void load_verts(py::array_t<double> verts_np)
    {
        int n_verts = verts_np.shape(0);

        vertices.resize(n_verts);
        auto r0 = verts_np.unchecked<2>();
    #pragma omp parallel for schedule(static) if(n_verts > 20480)
        for (int i = 0; i < n_verts; i++)
        {
            vertices[i].p.x = r0(i, 0);
            vertices[i].p.y = r0(i, 1);
            vertices[i].p.z = r0(i, 2);
        }
    }

    void load_faces(py::array_t<int> faces_np)
    {
        int n_faces = faces_np.shape(0);

        triangles.resize(n_faces);
        auto r0 = faces_np.unchecked<2>();
    #pragma omp parallel for schedule(static) if(n_faces > 20480)
        for (int i = 0; i < n_faces; i++)
        {
            triangles[i].v[0] = r0(i, 0);
            triangles[i].v[1] = r0(i, 1);
            triangles[i].v[2] = r0(i, 2);
            triangles[i].attr = 0;
            triangles[i].material = -1;
        }
    }

    void setMesh(py::array_t<double> verts_np, py::array_t<int> faces_np)
    {
        load_verts(verts_np);
        load_faces(faces_np);
    }

    py::array_t<double> np_getVertices()
    {
        int n_verts = vertices.size();

        std::vector<double> verts(n_verts*3);
    #pragma omp parallel for schedule(static) if(n_verts > 20480)
        for (int i = 0; i < n_verts; i++)
        {
            verts[i*3] = vertices[i].p.x;
            verts[i*3+1] = vertices[i].p.y;
            verts[i*3+2] = vertices[i].p.z;
        }

        py::array_t<double> verts_np({n_verts, 3}, (double*)verts.data());
        return verts_np;
    }

    py::array_t<int> np_getFaces()
    {
        int n_faces = triangles.size();

        std::vector<int> faces(n_faces*3);
    #pragma omp parallel for schedule(static) if(n_faces > 20480)
        for (int i = 0; i < n_faces; i++)
        {
            faces[i*3] = triangles[i].v[0];
            faces[i*3+1] = triangles[i].v[1];
            faces[i*3+2] = triangles[i].v[2];
        }

        py::array_t<int> faces_np({n_faces, 3}, (int*)faces.data());
        return faces_np;
    }

    py::array_t<double> np_getNormals()
    {
        int n_faces = triangles.size();

        std::vector<double> normals(n_faces*3);
    #pragma omp parallel for schedule(static) if(n_faces > 20480)
        for (int i = 0; i < n_faces; i++)
        {
            normals[i*3] = triangles[i].n.x;
            normals[i*3+1] = triangles[i].n.y;
            normals[i*3+2] = triangles[i].n.z;
        }

        py::array_t<double> normals_np({n_faces, 3}, (double*)normals.data());
        return normals_np;
    }

    py::tuple getMesh()
    {
        py::array_t<double> verts_np = np_getVertices();
        py::array_t<int> faces_np = np_getFaces();
        py::array_t<double> normals_np = np_getNormals();

        return py::make_tuple(verts_np, faces_np, normals_np);
    }

    void simplify_mesh_warpper(
        int target_count, 
        int update_rate = 5, 
        double aggressiveness = 7,
        double alpha = 1e-9, 
        int K = 3, 
        int max_iterations = 100,
        double threshold_lossless = 1e-4,
        bool lossless = false, 
        bool preserve_border = false, 
        bool verbose = false
    ) {
        /* 
        Simplify mesh

        Parameters
        ----------
        target_count : int
            Target number of triangles, not used if lossless is True
        update_rate : int
            Number of iterations between each update.
            If lossless flag is set to True, rate is 1
        aggressiveness : float
            Parameter controlling the growth rate of the threshold at each
            iteration when lossless is False.
        max_iterations : int
            Maximal number of iterations
        verbose : bool
            control verbosity
        lossless : bool
            Use the lossless simplification method
        threshold_lossless : float
            Maximal error after which a vertex is not deleted, only for
            lossless method.
        alpha : float
            Parameter for controlling the threshold growth
        K : int
            Parameter for controlling the thresold growth
        preserve_border : Bool
            Flag for preserving vertices on open border

        Note
        ----
        threshold = alpha*pow(iteration+K, agressiveness)
        */
        simplify_mesh(
            target_count, 
            update_rate, 
            aggressiveness, 
            alpha, 
            K, 
            max_iterations, 
            threshold_lossless, 
            lossless, 
            preserve_border, 
            verbose
        );
    }
}

PYBIND11_MODULE(core, m) {
    m.def("setMesh", &Simplify::setMesh, "Set mesh vertices and faces");
    m.def("getMesh", &Simplify::getMesh, "Get mesh vertices and faces");
    m.def("simplify_mesh_warpper", &Simplify::simplify_mesh_warpper, "Simplify mesh", 
        py::arg("target_count"),
        py::arg("update_rate") = 5, 
        py::arg("aggressiveness") = 7,
        py::arg("alpha") = 1e-9, 
        py::arg("K") = 3, 
        py::arg("max_iterations") = 100,
        py::arg("threshold_lossless") = 1e-4, 
        py::arg("lossless") = false,
        py::arg("preserve_border") = false, 
        py::arg("verbose") = false
    );
#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
