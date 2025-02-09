package org.example

import java.io.File
import kotlin.math.pow

class Encoder {
    private var sourceData = ""

    fun setDataFromFile(pathToFile: String): Encoder {
        return setDataFromString(File(pathToFile).readText())
    }

    fun setDataFromString(data: String): Encoder {
        sourceData = data

        return this
    }

    fun getSourceData(): String {
        return sourceData
    }

    fun encode(controlBits: Number): List<Boolean> {
        if (sourceData.isEmpty()) return emptyList()
        // получение списка чисел
        val numberSequence = lzwCompressAscii(sourceData)
        // выравнивание и получение последовательности бит
        val bitSequence = numbersCompress(numberSequence)
        // вес символа
        val lengthOfElemBitSeq = bitSequence.size / numberSequence.size
        // получение помехоустойчивого кода
        val bitSequenceResistance = genHammingCode(bitSequence, controlBits.toInt())
        // формирование метаданных
        val metadata = genHammingCode(
            numbersCompress(
                listOf(
                    controlBits.toInt(),
                    lengthOfElemBitSeq,
                    16
                )
            ).subList(0, 10)
        )
        // единная последовательность бит метаданные+данныеы
        return metadata.toMutableList().apply { addAll(bitSequenceResistance) }
    }

    private fun genHammingCode(
        bitSequence: List<Boolean>,
        controlBits: Int,
    ): List<Boolean> {
        val blockSize: Int = (2.0.pow(controlBits) - controlBits - 1).toInt()
        val buf = mutableListOf<List<Boolean>>().apply {
            for (i in 0..<(bitSequence.size / blockSize)) {
                add(bitSequence.subList(i * blockSize, i * blockSize + blockSize).toMutableList())
            }
            if (bitSequence.size % blockSize != 0) {
                add(bitSequence.subList(bitSequence.size / blockSize * blockSize, bitSequence.size).toMutableList())
            }
        }

        val res = mutableListOf<List<Boolean>>()
        buf.forEach {
            res.add(genHammingCode(it))
        }
        return res.flatten()
    }

    private fun genHammingCode(bitSequence: List<Boolean>): List<Boolean> {
        val block = mutableListOf<Boolean>().apply { addAll(bitSequence) }

        // add parity bits
        var pbit = 0
        while (1.shl(pbit) - 1 < block.size) {
            block.add(1.shl(pbit) - 1, false)
            pbit++
        }

        // init parity bits
        pbit = 0
        while (1.shl(pbit) - 1 < block.size) {
            var isCount = true
            var isParity = true
            for (i in 1.shl(pbit) - 1..<block.size step 1.shl(pbit)) {
                for (j in i..<i + 1.shl(pbit)) {
                    if (j >= block.size) {
                        break
                    }
                    if (isCount) {
                        isParity = if (block[j]) !isParity else isParity
                    }
                }
                isCount = !isCount
            }
            block[1.shl(pbit) - 1] = !isParity
            pbit++
        }

        return block
    }

    private fun lzwCompressAscii(data: String): List<Int> {
        val dictionary = mutableMapOf<String, Int>().apply {
            for (i in 0..<128) put(i.toChar().toString(), i.toInt())
        }
        val res = mutableListOf<Int>()
        var buf = ""
        for (c in data) {
            val wc = buf + c
            if (dictionary.containsKey(wc)) {
                buf = wc
            } else {
                res.add(dictionary[buf]!!)
                dictionary[wc] = dictionary.size
                buf = c.toString()
            }
        }

        if (buf.isNotEmpty()) res.add(dictionary[buf]!!)

        return res
    }

    private fun numbersCompress(data: List<Int>): List<Boolean> {
        val sequences = data.map {
            it.toBooleans().toMutableList()
        }
        val maxLengthOfNumber = sequences.maxOf { it.size }
        val res = sequences.map { sequence ->
            sequence.apply {
                repeat(maxLengthOfNumber - sequence.size) {
                    sequence.add(0, false)
                }
            }
        }.flatten()

        return res
    }

    fun main() {
        val encoder = Encoder()
//    val testData = "a"
//    val encodedData = encoder.setDataFromString(testData).encode(2)
//    println(testData)
//    val decodedData = Decoder().setEncodedData(encodedData).decode()
//    println(decodedData)
//
//    val testFile = "C:\\Users\\dmitr\\Projects\\2kurs2sem\\InformTheor\\rgz\\src\\main\\kotlin\\src.txt"
//    val encodedData1 = encoder.setDataFromFile(testFile).encode(5).setErrors(7)
////    println(encodedData)
//    println("compr: ${encodedData1.size} source: ${File(testFile).readText().length * 7}")
//    val decodedData1 = Decoder().setEncodedData(encodedData1).decode()
//    println(decodedData1)


        val testData2 =
            "a".repeat(1000)
        val encodedData2 = encoder.setDataFromString(testData2).encode(3).toMutableList()
        val posWithErrors2 = encodedData2.setErrors(7)
//    println(encodedData2)
        println(posWithErrors2)
//    val i = 13
//    encodedData2[i] = !encodedData2[i]
        println("compr: ${encodedData2.size} source: ${testData2.length * 7}")
        val decodedData2 = Decoder().setEncodedData(encodedData2).decode()
        println(decodedData2)
    }
}

fun List<Boolean>.toBits(): String =
    StringBuilder().apply {
        for (i in this@toBits) {
            append(if (i) '1' else '0')
        }
    }.toString()