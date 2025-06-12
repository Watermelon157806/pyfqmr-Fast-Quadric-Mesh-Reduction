pyfqmr : Python Fast Quadric Mesh Reduction
===========================================


Requirements:
~~~~~~~~~~~~~

-  *Numpy*
-  *Pybind11* (using 2.13.6)


Compilation :
~~~~~~~~~~~~~

Run:

.. code:: bash

    pip install .

Usage:
~~~~~~

.. code:: python

    >>> # We assume you have a numpy based mesh processing software
    >>> # Where you can get the vertices and faces of the mesh as numpy arrays.
    >>> # For example Trimesh or meshio
    >>> import pyfqmr
    >>> import trimesh as tr
    >>> bunny = tr.load_mesh('example/Stanford_Bunny_sample.stl')
    >>> # Simplify object
    >>> verts_out, faces_out = pyfqmr_v1.simplify(
    >>>     bunny.vertices.astype(np.float64), 
    >>>     bunny.faces.astype(np.int32), 
    >>>     target_count=200000, 
    >>>     aggressiveness=4, 
    >>>     preserve_border=True, 
    >>>     max_iterations=50, 
    >>>     verbose=True
    >>> )


Controlling the reduction algorithm
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Parameters of the **`simplify\_mesh`** method that can be tuned.

-  **target\_count**
    Target number of triangles.
-  **update\_rate**
    Number of iterations between each update.
-  **max\_iterations**
    Maximal number of iterations
-  **aggressiveness**
    Parameter controlling the growth rate of the threshold at each iteration when lossless is False.
-  **preserve\_border**
    Flag for preserving the vertices situated on open borders. Applies the method described in `this issue <https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification/issues/14>`__.
-  **alpha**
    Parameter for controlling the threshold growth. Exact implication described below.
-  **K**
    Parameter for controlling the threshold growth. Exact implication described below.
-  **lossless**
    Flag for using the lossless simplification method. Sets the update rate to 1 .
-  **threshold\_lossless**
    Maximal error after which a vertex is not deleted, only when the lossless flag is set to True.
-  **verbose**
    Falg controlling verbosity

$$threshold = alpha \* (iteration + K)^{agressiveness}$$


More information is to be found on Sp4cerat's repository :
""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
`Fast-Quadric-Mesh-Simplification <https://github.com/sp4cerat/Fast-Quadric-Mesh-Simplification>`__

Huge thanks to Sp4cerat for making his code available!
