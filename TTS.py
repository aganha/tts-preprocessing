#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random, string, os, sys

# Creamos un directorio temporario donde escribir todas las salidas.
TMP_DIR = '/tmp/tmp-' + ''.join(random.choice(string.lowercase) for i in range(10)) + '/'
os.system('mkdir ' + TMP_DIR)
print 'El output sera grabado en', TMP_DIR

if len(sys.argv) == 1:
  print
  print "Sistema de Síntesis del Habla"
  print "Departamento de Computación - Facultad de Ciencias Exactas y Naturales - Universidad de Buenos Aires"
  print
  print "Se requiere la URL de la página de Wikipedia a sintetizar: "
  print
  print "Sintaxis: ./TTS.py URL [voz] " 
  print
  print "Voces: "
  print "		Festival_UBA   (selección de unidades, base SECYT)"
  print "		Festival_el_diphone (español neutro)"
  print "		MaryTTS_UBA1   (selección de unidades, base SECYT)"
  print "		MaryTTS_UBA2   (HMM, base SECYT)" 
  print
  print "Ejemplo: ./TTS.py http://es.wikipedia.org/wiki/Universidad_de_Buenos_Aires Festival_UBA"
  print
  sys.exit(1)

# Leemos la voz elegida. Por defecto, es Festival HMM.
voz_elegida = "uba_spanish_arg_secyt_cg"
if len(sys.argv) >= 3:
  if sys.argv[2] == "Festival_UBA":
    voz_elegida = "uba_spanish_arg_secyt_cg"
  elif sys.argv[2] == "Festival_el_diphone":
    voz_elegida = "el_diphone"
  elif sys.argv[2] == "MaryTTS_UBA1":
    voz_elegida = "uba_secyt"
  elif sys.argv[2] == "MaryTTS_UBA2":
    voz_elegida = "uba_secyt-hsmm"

# Preparamos la secuencia de comandos a ejecutar.
cmd = ''

# Traemos la página de Wikipedia (el proxy es si estamos
# dentro de *.exp.dc.uba.ar)
cmd += 'export http_proxy="http://proxy.fcen.uba.ar:8080";'
cmd += 'cd HTML2TXT-0.3/; wget -qO- "%s" | ./HTML2TXT -o %s/HtmlToTxt.txt ; cd ..;' % (sys.argv[1], TMP_DIR)

# Separa en oraciones.
cmd += 'cd SeparaOraciones-0.3/; ./SeparaOraciones -i %s/HtmlToTxt.txt -o %s/TextoSeparado.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Traduce palabras extranjeras.
cmd += 'cd Traductor-0.3/; ./Traductor -i %s/TextoSeparado.txt -o %s/TextoTraducido.txt ; cd ..;' % (TMP_DIR, TMP_DIR)

# Normaliza el texto.
cmd += 'cd Normalizador-0.3/; ./Normalizador -i %s/TextoTraducido.txt -o %s/SalidaNormalizador.txt  > /dev/null; cd ..;' % (TMP_DIR, TMP_DIR)

# Para Festival hace falta hacer algunos cambios. Ejemplos: "á"-->"'a", "fin."-->"fin ."
if voz_elegida=="uba_spanish_arg_secyt_cg" or voz_elegida=="el_diphone":
  cmd += 'cd BackEndTransf-0.3/; ./BackEndTransf -i %s/SalidaNormalizador.txt -o %s/SalidaBackEndTransf.txt ; cd ..;' % (TMP_DIR, TMP_DIR)
else:
  cmd += 'cp %s/SalidaNormalizador.txt %s/SalidaBackEndTransf.txt;' % (TMP_DIR, TMP_DIR)

# Síntesis propiamente dicha, con Festival o Mary TTS.
cmd += 'cd BackEnd-0.3/; ./BackEnd -i %s/SalidaBackEndTransf.txt -o %s -v %s; cd ..;' % (TMP_DIR, TMP_DIR, voz_elegida)

# Ejecutamos los comandos.
os.system(cmd)

