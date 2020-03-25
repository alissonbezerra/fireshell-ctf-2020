package com.arconsultoria.cars.domain

import java.io.Serializable

data class Car (
    var id: Int,

    var name: String,

    var year: Int,

    var image: String
) : Serializable