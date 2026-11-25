{
  description = "Library for exploratory landscape analysis";

  inputs = {
    nixpkgs.url = "https://channels.nixos.org/nixpkgs-unstable/nixexprs.tar.zst";
  };

  outputs = inputs: {
    packages = builtins.mapAttrs (system: pkgs: {
    }) inputs.nixpkgs.legacyPackages;

    devShells = builtins.mapAttrs (system: pkgs: {
        default = pkgs.mkShellNoCC {
            packages = with pkgs; [
                cmake
                gcc
                llvmPackages.clang-tools
                cppcheck
            ];
        };
    }) inputs.nixpkgs.legacyPackages;
  };
}
