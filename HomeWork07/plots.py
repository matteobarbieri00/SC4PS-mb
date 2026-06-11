from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np


def plot_coin_convergence(path: Path) -> None:
    if not path.exists():
        raise FileNotFoundError(f"Coin data not found: {path}")

    data = np.loadtxt(path)
    if data.ndim != 2 or data.shape[1] != 2:
        raise ValueError("coin_running_fraction.txt must have 2 columns: toss_index running_fraction")

    toss_index = data[:, 0]
    running_fraction = data[:, 1]

    plt.figure(figsize=(9, 5))
    plt.semilogx(toss_index, running_fraction, label="Running fraction of heads", lw=1.2)
    plt.axhline(0.5, color="tab:red", linestyle="--", lw=1.2, label="Expected value = 0.5")

    plt.title("Convergence of Running Heads Fraction")
    plt.xlabel("Number of tosses")
    plt.ylabel("Running fraction of heads")
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()

    output_path = Path("./outputs/coin_convergence.png")
    plt.savefig(output_path, dpi=150)
    print(f"Saved plot to {output_path}")


def plot_pi_error(path: Path) -> None:
    if not path.exists():
        raise FileNotFoundError(f"Pi data not found: {path}")

    data = np.loadtxt(path)
    if data.ndim != 2 or data.shape[1] != 3:
        raise ValueError("pi_error_vs_n.txt must have 3 columns: N pi_estimate abs_error")

    n = data[:, 0]
    abs_err = data[:, 2]

    # Reference slope for Monte Carlo convergence: O(1/sqrt(N)).
    guide = abs_err[0] * np.sqrt(n[0] / n)

    plt.figure(figsize=(9, 5))
    plt.loglog(n, abs_err, "o-", label=r"$\left|\hat{\pi}-\pi\right|$", lw=1.2)
    plt.loglog(n, guide, "--", label=r"$\propto N^{-1/2}$ guide", lw=1.2)

    plt.title("Monte Carlo Pi Error vs Sample Size")
    plt.xlabel("N")
    plt.ylabel("Absolute error")
    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    output_path = Path("./outputs/pi_error_convergence.png")
    plt.savefig(output_path, dpi=150)
    print(f"Saved plot to {output_path}")


def plot_u_squared_density(path: Path) -> None:
    if not path.exists():
        raise FileNotFoundError(f"U^2 data not found: {path}")

    data = np.loadtxt(path)
    if data.ndim != 2 or data.shape[1] != 3:
        raise ValueError("u_squared_samples.txt must have 3 columns: u y_square y_exp")

    y_square = data[:, 1]
    y_exp = data[:, 2]
    lambda_rate = 1.5

    fig, axes = plt.subplots(1, 2, figsize=(12, 5))

    # Subplot 1: change of variables Y = U^2.
    axes[0].hist(y_square, bins=80, density=True, alpha=0.6, label=r"Histogram of $Y=U^2$")
    y_grid = np.linspace(1e-4, 1.0, 1000)
    f_y_square = 1.0 / (2.0 * np.sqrt(y_grid))
    axes[0].plot(y_grid, f_y_square, "r-", lw=2, label=r"$f_Y(y)=\frac{1}{2\sqrt{y}}$")
    axes[0].set_title(r"Change of Variables: $Y=U^2$")
    axes[0].set_xlabel("y")
    axes[0].set_ylabel("Density")
    axes[0].grid(True, alpha=0.3)
    axes[0].legend()

    # Subplot 2: inverse transform for exponential distribution.
    axes[1].hist(y_exp, bins=80, density=True, alpha=0.6, label="Histogram (inverse transform)")
    y_max = np.quantile(y_exp, 0.995)
    y_grid_exp = np.linspace(0.0, y_max, 1000)
    f_y_exp = lambda_rate * np.exp(-lambda_rate * y_grid_exp)
    axes[1].plot(y_grid_exp, f_y_exp, "r-", lw=2, label=rf"$f_Y(y)={lambda_rate}\,e^{{-{lambda_rate}y}}$")
    axes[1].set_title("Inverse Transform: Exponential")
    axes[1].set_xlabel("y")
    axes[1].set_ylabel("Density")
    axes[1].grid(True, alpha=0.3)
    axes[1].legend()

    fig.tight_layout()

    output_path = Path("./outputs/u_transform_subplots.png")
    plt.savefig(output_path, dpi=150)
    print(f"Saved plot to {output_path}")


def plot_exponential_empirical_cdf(path: Path, lambda_rate: float = 1.5) -> None:
    if not path.exists():
        raise FileNotFoundError(f"Empirical CDF data not found: {path}")

    data = np.loadtxt(path)
    if data.ndim != 2 or data.shape[1] != 2:
        raise ValueError("exp_empirical_cdf.txt must have 2 columns: x_i empirical_cdf")

    x = data[:, 0]
    ecdf = data[:, 1]
    exact_cdf = 1.0 - np.exp(-lambda_rate * x)

    plt.figure(figsize=(9, 5))
    plt.step(x, ecdf, where="post", lw=1.2, label="Empirical CDF")
    plt.plot(x, exact_cdf, "r--", lw=2, label=rf"Exact CDF: $1-e^{{-{lambda_rate}x}}$")

    plt.title("Empirical vs Exact CDF (Exponential Sample)")
    plt.xlabel("x")
    plt.ylabel("CDF")
    plt.ylim(0.0, 1.02)
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()

    output_path = Path("./outputs/exp_empirical_cdf.png")
    plt.savefig(output_path, dpi=150)
    print(f"Saved plot to {output_path}")


def main() -> None:
    plot_coin_convergence(Path("./outputs/coin_running_fraction.txt"))
    plot_pi_error(Path("./outputs/pi_error_vs_n.txt"))
    plot_u_squared_density(Path("./outputs/u_squared_samples.txt"))
    plot_exponential_empirical_cdf(Path("./outputs/exp_empirical_cdf.txt"))


if __name__ == "__main__":
    main()
