#!/usr/bin/env bash
reset

echo " "
echo "**Compilando"

make tester
if [ $? -ne 0 ]; then
  echo "  **Error de compilacion"
  exit 1
fi

echo " "
echo "**Corriendo Valgrind"

command -v valgrind > /dev/null
if [ $? -ne 0 ]; then
  echo "ERROR: No se encuentra valgrind."
  exit 1
fi

valgrind --show-reachable=yes --leak-check=full --error-exitcode=1 ./tester
if [ $? -ne 0 ]; then
  echo "  **Error de memoria"
  exit 1
fi

echo " "
echo "**Corriendo diferencias con la catedra"

DIFFER="diff -d"
ERRORDIFF=0

$DIFFER salida.propios.chk2.txt salida.catedra.chk2.txt > /tmp/diff1
if [ $? -ne 0 ]; then
  echo "  **Discrepancia en el checkpoint 2"
  ERRORDIFF=1
fi

$DIFFER salida.propios.chk3.txt salida.catedra.chk3.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancia en el checkpoint 3"
  ERRORDIFF=1
fi

$DIFFER salida.propios.chk4.txt salida.catedra.chk4.txt > /tmp/diff2
if [ $? -ne 0 ]; then
  echo "  **Discrepancia en el checkpoint 4"
  ERRORDIFF=1
fi

echo " "
if [ $ERRORDIFF -eq 0 ]; then
  echo "**Todos los tests pasan"
fi
echo " "

