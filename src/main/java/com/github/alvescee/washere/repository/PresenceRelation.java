package com.github.alvescee.washere.repository;

import java.util.UUID;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;

import com.github.alvescee.washere.model.Lesson;
import com.github.alvescee.washere.model.Presence;
import com.github.alvescee.washere.model.Student;

import jakarta.transaction.Transactional;

public interface PresenceRelation extends JpaRepository<Presence, UUID> {

    @Modifying
    @Transactional
    @Query("UPDATE Presence p SET p.isPresent = true WHERE p.lesson = :lesson AND p.student = :student")
    public void setPresenceByChipAndDate(@Param("lesson") Lesson lesson, @Param("student") Student student);

    @Query("SELECT p.isPresent FROM Presence p WHERE p.lesson = :lesson AND p.student = :student")
    public boolean yetPresent (@Param("lesson") Lesson lesson, @Param("student") Student student);
}