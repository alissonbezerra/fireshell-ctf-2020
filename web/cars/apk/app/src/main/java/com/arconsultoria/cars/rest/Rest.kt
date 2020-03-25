package com.arconsultoria.cars.rest

import com.arconsultoria.cars.domain.Car
import com.arconsultoria.cars.domain.Comment
import com.arconsultoria.cars.domain.CommentResponse
import retrofit2.Call
import retrofit2.http.Body
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.Path

interface Rest {

    @GET("/cars")
    fun getCars(): Call<List<Car>>

    @GET("/car/{id}")
    fun getCar(@Path("id") id: Int): Call<Car>

    @POST("/comment")
    fun postComment(@Body comment: Comment): Call<CommentResponse>

}