# from .Simplify import *
import time
import numpy as np
from . import core as _C

def simplify(verts, faces, target_count=200000, aggressiveness=4, preserve_border=True, max_iterations=50, verbose=True):
    """
    Simplify a mesh using the fqmr algorithm.

    Parameters:
        verts (numpy.ndarray): Vertices of the mesh.
        faces (numpy.ndarray): Faces of the mesh.
        target_count (int): Target number of vertices after simplification.
        aggressiveness (int): Aggressiveness of the simplification.
        preserve_border (bool): Whether to preserve border edges.
        verbose (bool): Whether to print progress information.
        max_iterations (int): Maximum number of iterations for simplification.

    Returns:
        tuple: Simplified vertices and faces.
    """
    t0 = time.time()
    _C.setMesh(verts.astype(np.float64), faces.astype(np.int32))
    t1 = time.time()
    if verbose:
        print(f"Time taken to set mesh: {t1 - t0:.4f} seconds")

    t0 = time.time()
    _C.simplify_mesh_warpper(
        target_count = target_count, 
        update_rate = 5, 
        aggressiveness = aggressiveness,
        alpha = 1e-9, 
        K = 3, 
        max_iterations = max_iterations,
        threshold_lossless = 1e-4,
        lossless = False, 
        preserve_border = preserve_border, 
        verbose = verbose,
    )
    t1 = time.time()

    if verbose:
        print(f"Time taken for simplification: {t1 - t0:.4f} seconds")

    t0 = time.time()
    res_verts, res_faces, _ = _C.getMesh()
    t1 = time.time()

    if verbose:
        print(f"Time taken to get mesh: {t1 - t0:.4f} seconds")

    if verbose:
        print(f"Original mesh: {faces.shape[0]} faces, {verts.shape[0]} vertices")
        print(f"Simplified mesh: {res_faces.shape[0]} faces, {res_verts.shape[0]} vertices")

    return res_verts, res_faces
