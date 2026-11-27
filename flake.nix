{
  description = "Library for exploratory landscape analysis";

  inputs = {
    nixpkgs.url = "https://channels.nixos.org/nixpkgs-unstable/nixexprs.tar.zst";
  };

  outputs = inputs: {
    packages = builtins.mapAttrs (system: pkgs: {
      orograph = pkgs.stdenv.mkDerivation {
        pname = "orograph";
        version = "0.0.0";

        src = ./.;

        nativeBuildInputs = with pkgs; [ cmake ];
        buildInputs = with pkgs; [ eigen_5 ];
      };

      default = inputs.self.packages.${system}.orograph;
    }) inputs.nixpkgs.legacyPackages;

    devShells = builtins.mapAttrs (system: pkgs: {
      default = pkgs.mkShellNoCC {
        packages = with pkgs; [
          cmake
          gcc
          llvmPackages.clang-tools
          cppcheck
          eigen_5
        ];
      };
    }) inputs.nixpkgs.legacyPackages;
  };
}
