package org.example

import java.io.File

fun main() {
    val encoder = Encoder()
    val decoder = Decoder()
    var isGeneratorErrors = false
    println("add generator of errors (y)es or (n)o")
    when (readlnOrNull()) {
        "y" -> isGeneratorErrors = true
        "n" -> isGeneratorErrors = false
    }

    while (true) {
        println("Change mode to (e)ncode or (d)ecode")
        var mode = readlnOrNull()
        val codeMode = if (mode == "e") "e" else if (mode == "d") "d" else continue

        println("Change mode of reading from (f)ile or (s)tring")
        mode = readlnOrNull()
        val inputMode = if (mode == "f") "f" else if (mode == "s") "s" else continue

        println("Change mode of writing to (f)ile or (s)tring")
        mode = readlnOrNull()
        val outputMode = if (mode == "f") "f" else if (mode == "s") "s" else continue

        when (codeMode) {
            "e" -> {
                when (inputMode) {
                    "f" -> {
                        println("Enter path to file to encode")
                        encoder.setDataFromFile(readln())
                    }

                    "s" -> {
                        println("Enter data to encode")
                        encoder.setDataFromString(readln())
                    }

                    else -> {
                        continue
                    }
                }

                println("input num of pbits")
                val pbits = readln().toInt()

                val encodedData = encoder.encode(pbits).toMutableList()
                if (isGeneratorErrors) {
                    println("input num of errors")
                    val numOferrors = readln().toInt()
                    println("error bits on positions: ${encodedData.setErrors(numOferrors)}")
                }

                when (outputMode) {
                    "f" -> {
                        println("Enter path to file to write encoded data")
                        val f = File(readln()).writer()
                        f.write(encodedData.toBits())
                        f.close()
                    }

                    "s" -> {
                        println("Encoded data:")
                        println(encodedData.toBits())
                    }

                    else -> {
                        continue
                    }
                }

                println("source: ${encoder.getSourceData().length * 7}")
                println("compressed: ${encodedData.size}")
            }

            "d" -> {
                when (inputMode) {
                    "f" -> {
                        println("Enter path to file to decode")
                        decoder.setEncodedDataFromFile(readln())
                    }

                    "s" -> {
                        println("Enter data to decode")
                        decoder.setEncodedDataFromString(readln())
                    }

                    else -> {
                        continue
                    }
                }

                val decodedData = try {
                    decoder.decode()
                } catch (e: NotCorrectedByPBitException) {
                    println("Transport error")
                    continue
                }

                when (outputMode) {
                    "f" -> {
                        println("Enter path to file to write decoded data")
                        val f = File(readln()).writer()
                        f.write(decodedData)
                        f.close()
                    }

                    "s" -> {
                        println("Decoded data:")
                        println(decodedData)
                    }

                    else -> {
                        continue
                    }
                }
            }
        }
    }
}