for file in **/*.png; do pngcrush "$file" "${file%.png}-crushed.png" && mv "${file%.png}-crushed.png" "$file"; done
