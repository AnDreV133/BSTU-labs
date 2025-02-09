package org.example

object test {
    data class Test(val a: Int, val b: Int) : TestSuper(a, b) {

    }

    open class TestSuper(a: Int, b: Int) {

    }
}