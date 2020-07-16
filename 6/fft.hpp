#include <fstream>
#include <cassert>
#include <complex>

// fft implementation based in part on code from:
// https://stackoverflow.com/questions/8801158/fft-in-a-single-c-file

class discrete_function
{
private:
    uint size;
    uint logSize;
    double d;
    std::complex<double> *vec;
    std::complex<double> (*func)(double);

    int reverse(int n) //calculating reverse number
    {
        uint j, p = 0;
        for (j = 1; j <= logSize; j++)
        {
            if (n & (1 << (logSize - j)))
                p |= 1 << (j - 1);
        }
        return p;
    }

    void ordina() //using the reverse order in the array
    {
        std::complex<double> f2[size];
        for (uint i = 0; i < size; i++)
            f2[i] = vec[reverse(i)];
        for (uint j = 0; j < size; j++)
            vec[j] = f2[j];
    }

    void transformPrivate(int sign)//transforms the array +1:fft -1:rfft
    {
        ordina(); // first: reverse order
        std::complex<double> *W;
        W = (std::complex<double> *)malloc(size / 2 * sizeof(std::complex<double>));
        W[1] = std::polar(1., -2. * sign * M_PI / size);
        W[0] = 1;
        for (uint i = 2; i < size / 2; i++)
            W[i] = pow(W[1], i);
        int n = 1;
        int a = size / 2;
        for (uint j = 0; j < logSize; j++)
        {
            for (uint i = 0; i < size; i++)
            {
                if (!(i & n))
                {
                    std::complex<double> temp = vec[i];
                    std::complex<double> Temp = W[(i * a) % (n * a)] * vec[i + n];
                    vec[i] = temp + Temp;
                    vec[i + n] = temp - Temp;
                }
            }
            n *= 2;
            a = a / 2;
        }
        free(W);
    }

public:
    discrete_function &reset()
    {
        for (uint i = 0; i < size; i++)
        {
            vec[i] = func(i * d);
        }
        return *this;
    }
    discrete_function &norm()
    {
        for (uint i = 0; i < size; i++)
        {
            vec[i] = std::norm(vec[i]);
        }
        return *this;
    }
    discrete_function &conj()
    {
        for (uint i = 0; i < size; i++)
        {
            vec[i] = std::conj(vec[i]);
        }
        return *this;
    }

    //overloads the *= operator to multiply functions
    discrete_function &operator*=(discrete_function &input)
    {
        assert(size == input.getSize());
        for (uint i = 0; i < size; i++)
        {
            vec[i] *= input[i];
        }
        return *this;
    }

    //overloads the *= operator to scale functions
    discrete_function &operator*=(std::complex<double> a)
    {
        for (uint i = 0; i < size; i++)
        {
            vec[i] *= a;
        }
        return *this;
    }

    discrete_function &rfft()
    {
        transformPrivate(-1);
        for (uint i = 0; i < size; i++)
            vec[i] *= d;
        return *this;
    }

    discrete_function &fft()
    {
        transformPrivate(1);
        for (uint i = 0; i < size; i++)
            vec[i] *= d;
        return *this;
    }

    uint getSize()
    {
        return size;
    }

    std::complex<double> operator[](uint i)
    {
        return vec[i];
    }

    void plot(std::string fileName)
    {
        std::ofstream outFile;
        outFile.open(fileName);
        for (uint i = 0; i < size; i++)
        {
            outFile << d * i << " " << vec[i].real() << "\n";
        }
        outFile.close();
        outFile.clear();
    }

    discrete_function(std::complex<double> (*f)(double), uint size, double range);
    ~discrete_function();
};

discrete_function::discrete_function(std::complex<double> (*f)(double), uint _n, double range)
{
    size = 1 << _n;
    logSize = _n;
    d = range/size;
    func = f;
    vec = (std::complex<double> *)malloc(size * sizeof(std::complex<double>));
    for (uint i = 0; i < size; i++)
    {
        vec[i] = f(i * d);
    }
}

discrete_function::~discrete_function()
{
    free(vec);
}