# Answer to the LESSONS.md for HomeWork 4

I will here answer to the questions, dividing them in parts in order to easily map answers to questions.

## Part 1

1. The FFT is the efficient algortithm for the DFT: while DFT requires $\mathcal{O}(N^2)$ operations, the FFT requires $\mathcal{O}(N\log_2N)$. 
What they do is to take a discrete set of points (for instance digital samples) and calculate their frequency components. An example to explain this is to take a generic sound, composed by different sound waves with different frequencies. The DFT (and thus the FFT) brakes it down into its individual frequency components, namely it giveds the frequecy of each sound wave composing the complex sound.

2. The *sampling rate* is how many times per second one "looks" at the signal to take a measurement, whereas the *Nyquist frequency* is the highest frequency one can record with a given sampling rate ($f_N =f_s/2$). This is crucial for the aliasing problem. Finally the *frequency resolution* is the distance between the point of an FFT plot, which is ($ \Delta f = f_s/N$ ).

3. In a DFT it is assumed that each sample $x_n$ occurs at a time $t_n = n\Delta t$. The FFT is fast because it recursively breaks down the signal in smaller halves, which clearly relies on periodicity and symmetry.

## Part 2

1. The created csv files in the output directory are:
    - `output/coupled_oscillators_spectrum.csv`
    - `output/coupled_oscillators_time.csv`
    - `output/good_sampling_signal.csv`
    - `output/good_sampling_spectrum.csv`
    - `output/short_record_signal.csv`
    - `output/short_record_spectrum.csv`
    - `output/undersampled_signal.csv`
    - `output/undersampled_spectrum.csv`

2. `sampling_demo.c`

3. `coupled_oscillators.c`

4. $50$ Hz and $120$ Hz.

## Part 3

1. Not a question.

2. $f_N = f_s / 2 = 128/2 = 64$ Hz.

3. It cannot be reconstructed faithfully because of Shannon's theorem, namely $f_s < 2 \times 120$.

4. Not a question.

5. This phenomenon is called **aliasing**: a reconstructed signal from samples of the original signal contains low frequency components that are not present in the original one. This is caused when, in the original signal, there are components in the signal which are greater than $f_N$. Mathematically, sampling a continuos signal $x(t)$ is equivalent to multiplying it by a Dirac Comb $\sum_{n=-\infty}^{+\infty} \delta(t-nT_s)$, where $T_s$ is the sampling period. In the Frequency Domain, multiplication in time becomes convolution in frequency. This causes the original signal’s spectrum to repeat itself (to "copy-paste") at every multiple of the sampling frequency $f_s$.

## Part 4

1. Not a question.
2. $T = N\Delta t = \frac{N}{f_s} =  64/512 = 0.125$ s
3. $\Delta f = 1/T = 1/0.125 = 8$ Hz
4. Two distinguish two different frequencies in an FFT, the difference between them must be greater or equal to the frequency resolution. Here $55-50 = 5 < \Delta f = 8$.
5. I would increase the total acquisition time $T$, because this way $\Delta f$ will decrease allowing the resolution of the two frequencies.

## Part 5

1. $m\ddot{x_1} = -(k + k_c) x_1 + k_c x_2$

    $m\ddot{x_2} = k_c x_1 - (k+k_c)x_2$

2. Two identical coupled harmonic oscillators.
3. Symmetric $\omega_1 = \sqrt{k/m}$ and anti-symmetric $\omega_2 = \sqrt{(k+2k_c)/m}$.
4. The reason behind it is the coupling of the system. $x_1(t)$ is a linear combination of the two normal modes.
5. It is useful to validate the physical model.

## Part 6

1. `sampling_signals.png`
2. `sampling_spectra.png`
3. yes, but slight difference.
4. The reason is the discreteness of numerics, whereas the differential equations are of continuous nature.

## Part 7
I choose option A. More precisely I modify the sampling rate of the undersampled case from $128$ to $256$. I predict that:
1. Aliasing will not occur, because the $120$ frequency is below Nyquist.
2. The peak locations should be $50$ and $120$ Hz.

After running again, numerics confirm my prediction. The output is:

3. `The 120 Hz component is above Nyquist for fs = 256 Hz and folds back to an alias near 8 Hz.
  sampling rate = 256.0 Hz, Nyquist = 128.0 Hz, duration = 0.500 s
  FFT frequency resolution = 2.000 Hz
  dominant spectral peaks:
    peak 1: f =   50.000 Hz, amplitude = 1.0000
    peak 2: f =  120.000 Hz, amplitude = 0.7000
  wrote output/undersampled_signal.csv
  wrote output/undersampled_spectrum.csv`

## Part 8

From this project I have learnt how to use FFT to analyze signals. In particular, it gave a hands-on approach to theoretical topics such as the DFT and Shannon Theorem. 

Moreover, it gave me an example on how scientific software is organized. In particular on how to separate the source files from the include and on how to organize the output. However the most significant point is the layout of the Makefile.

Nevertheless i think that an important part is missing. Indeed for scientific computing it often happens that data is taken from a dataset. Therefore I think that a part where the input signals are taken from a dataset (even generated with a script), could be beneficial for learning the topic.

