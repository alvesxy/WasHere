package com.github.alvescee.washere.controller;

import java.util.Calendar;
import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PutMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.github.alvescee.washere.model.Lesson;
import com.github.alvescee.washere.model.Student;
import com.github.alvescee.washere.repository.LessonRelation;
import com.github.alvescee.washere.repository.PresenceRelation;
import com.github.alvescee.washere.repository.StudentRelation;

@RestController
@RequestMapping("/presence")
public class PresencePath {

    @Autowired
    private PresenceRelation presenceRelation;

    @Autowired
    private LessonRelation lessonRelation;

    @Autowired
    private StudentRelation studentRelation;
    
    @PutMapping("/{chip}")
    public ResponseEntity<?> change(@PathVariable long chip) {

        final Calendar today = Calendar.getInstance();
        final int day = today.get(Calendar.DAY_OF_MONTH);

        // Month goes 0-11, so I add 1 to make correctly month
        final int month = today.get(Calendar.MONTH) + 1;

        final Optional<Lesson> lesson = this.lessonRelation.findLessonByDayAndMonth(day, month);
        final Optional<Student> student = this.studentRelation.findByChip(chip);
        
        if (lesson.isEmpty()) return ResponseEntity.badRequest().build();
        if (student.isEmpty()) return ResponseEntity.notFound().build();

        if (this.presenceRelation.yetPresent(lesson.get(), student.get())) return ResponseEntity.status(HttpStatus.CONFLICT).build();

        this.presenceRelation.setPresenceByChipAndDate(lesson.get(), student.get());

        return ResponseEntity.ok().build();
    }
}