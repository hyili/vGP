#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <string>
#include <termios.h>

/*
 * the actual function we want
 */
static PyObject *getpassword(PyObject *self) {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);

    std::string inputBuffer;
    getline(std::cin, inputBuffer);

    std::cout << std::endl;

    // https://docs.python.org/3/c-api/bytearray.html
    return PyByteArray_FromStringAndSize(inputBuffer.c_str(),
                                         inputBuffer.size());
}

/*
 * binding interpreter and C++ library
 */
static PyMethodDef vGPMethods[] = {{"getpassword", (PyCFunction)getpassword,
                                    METH_NOARGS, "Get a bytearray password."},
                                   {NULL}};

static struct PyModuleDef vGP = {PyModuleDef_HEAD_INIT, "vGP",
                                 "volatile get password", -1, vGPMethods};

static PyObject *volatileError;
PyMODINIT_FUNC PyInit_vGP(void) {
    PyObject *module;

    module = PyModule_Create(&vGP);
    if (module == NULL)
        return NULL;

    volatileError = PyErr_NewException("volatile.error", NULL, NULL);
    Py_XINCREF(volatileError);

    if (PyModule_AddObject(module, "error", volatileError) < 0) {
        Py_XDECREF(volatileError);
        Py_CLEAR(volatileError);
        Py_DECREF(module);
        return NULL;
    }

    return module;
}

